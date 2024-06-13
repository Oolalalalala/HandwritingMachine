#include "TicTacToeState.h"

#include <Arduino.h>
#include "../Core/Esp32Program.h"
#include "../Core/IO.h"

#include "../Core/Camera.h"
#include "../Core/WritingMachine.h"
#include "../Core/WifiInterface.h"

#define BLOCK_LENGTH 30.0f
#define CV_HEADER_BYTE 'c'
#define CAMERA_OFFSET 78.0f

struct TicTacToeData
{
    unsigned char Board[3][3]; // 0 = empty, 1 = User, 2 = AI
    int IsUserCross;

    enum class State
    {
        Begin = 0, DrawBoard, PlayerTurn, ScanBoard, AITurn, GameOver
    };

    State GameState;
    
    Vector2Int ScanPos;
    Vector2 LastPos;
};

static TicTacToeData s_Data;


void TicTacToeState::OnEnter()
{
    IO::ClearDisplay();
    IO::DisplayMessage(0, "Tic Tac Toe");

    memset(s_Data.Board, 0, sizeof(s_Data.Board));
    s_Data.GameState = TicTacToeData::State::Begin;
    s_Data.IsUserCross = -1;

    // Draw board
    float x0 = 120.0f - BLOCK_LENGTH * 1.5f;
    float x1 = 120.0f - BLOCK_LENGTH * 0.5f;
    float x2 = 120.0f + BLOCK_LENGTH * 0.5f;
    float x3 = 120.0f + BLOCK_LENGTH * 1.5f;

    float y0 = 100.0f - BLOCK_LENGTH * 1.5f;
    float y1 = 100.0f - BLOCK_LENGTH * 0.5f;
    float y2 = 100.0f + BLOCK_LENGTH * 0.5f;
    float y3 = 100.0f + BLOCK_LENGTH * 1.5f;


    auto& commandBuffer = ESP32Program::Get().GetWritingMachine().GetCommandBuffer();
    commandBuffer.Move(Vector2{0.0f, 0.0f}, Vector2{x1, y0});
    commandBuffer.DrawLine(Vector2{x1, y0}, Vector2{x1, y3});
    commandBuffer.Move(Vector2{x1, y3}, Vector2{x2, y3});
    commandBuffer.DrawLine(Vector2{x2, y3}, Vector2{x2, y0});
    commandBuffer.Move(Vector2{x2, y0}, Vector2{x3, y1});
    commandBuffer.DrawLine(Vector2{x3, y1}, Vector2{x0, y1});
    commandBuffer.Move(Vector2{x0, y1}, Vector2{x0, y2});
    commandBuffer.DrawLine(Vector2{x0, y2}, Vector2{x3, y2});
    commandBuffer.Move(Vector2{x3, y2}, Vector2{10.0f, 180.0f});
    s_Data.LastPos = Vector2{10.0f, 180.0f};
}

void TicTacToeState::OnUpdate(float dt)
{
    IO::PullData();

    if (IO::IsButtonDown(Button::Cancel))
    {
        ESP32Program::Get().SwitchState(State::Menu);
        return;
    }

    auto& writingMachine = ESP32Program::Get().GetWritingMachine();
    auto& commandBuffer = writingMachine.GetCommandBuffer();

    switch (s_Data.GameState)
    {
        case TicTacToeData::State::Begin:
        {
            if (IO::IsButtonDown(Button::Enter))
            {
                s_Data.GameState = TicTacToeData::State::DrawBoard;
                writingMachine.Enable(true);
            }
            break;
        }
        case TicTacToeData::State::DrawBoard:
        {
            writingMachine.OnUpdate(dt);

            if (writingMachine.Finished())
                s_Data.GameState = TicTacToeData::State::PlayerTurn;
            break;
        }
        case TicTacToeData::State::PlayerTurn:
        { 
            if (ESP32Program::Get().CameraDisabled())
                ESP32Program::Get().SwitchState(State::Menu);
            if (IO::IsButtonDown(Button::Enter))
            {
                s_Data.ScanPos = Vector2Int{-1, -2};
                s_Data.GameState = TicTacToeData::State::ScanBoard;
            }
            break;
        }
        case TicTacToeData::State::ScanBoard:
        {
            if (writingMachine.Finished())
            {   
                int result = 0;
                if (s_Data.ScanPos.Y != -2)
                {
                    WifiInterface::WaitForClientConnection();
                    CaptureAndSendPhoto();
                    result = WaitAndAcceptForResponse();
                    WifiInterface::DumpClient();
                }

                do
                {
                    s_Data.ScanPos.Y++;
                    if (s_Data.ScanPos.Y == 2)
                    {
                        s_Data.ScanPos.Y = -1;
                        s_Data.ScanPos.X++;
                    }

                    if (s_Data.ScanPos.X == 2 || result == 1)
                    {
                        s_Data.GameState = TicTacToeData::State::AITurn;
                        
                        Vector2Int nextPos = DecideAction();
                        Vector2 center = {120.0f + BLOCK_LENGTH * (nextPos.X - 1), 100.0f + BLOCK_LENGTH * (nextPos.Y - 1)};
                        if (s_Data.IsUserCross == 1)
                        {
                            commandBuffer.Move(s_Data.LastPos, Vector2{ center.X - BLOCK_LENGTH / 3.0f, center.Y });
                            commandBuffer.DrawArc(center, BLOCK_LENGTH / 3.0f, 0, 2 * PI);
                            commandBuffer.Move(Vector2{ center.X - BLOCK_LENGTH / 3.0f, center.Y }, Vector2{ 10.0f, 180.0f });
                        }
                        else
                        {
                            commandBuffer.Move(s_Data.LastPos, Vector2{ center.X - BLOCK_LENGTH / 3.0f, center.Y - BLOCK_LENGTH / 3.0f });
                            commandBuffer.DrawLine(Vector2{ center.X - BLOCK_LENGTH / 3.0f, center.Y - BLOCK_LENGTH / 3.0f }, Vector2{ center.X + BLOCK_LENGTH / 3.0f, center.Y + BLOCK_LENGTH / 3.0f });
                            commandBuffer.Move(Vector2{ center.X + BLOCK_LENGTH / 3.0f, center.Y + BLOCK_LENGTH / 3.0f }, Vector2{ center.X + BLOCK_LENGTH / 3.0f, center.Y - BLOCK_LENGTH / 3.0f });
                            commandBuffer.DrawLine({ center.X + BLOCK_LENGTH / 3.0f, center.Y - BLOCK_LENGTH / 3.0f }, Vector2{ center.X - BLOCK_LENGTH / 3.0f, center.Y + BLOCK_LENGTH / 3.0f });
                            commandBuffer.Move(Vector2{ center.X - BLOCK_LENGTH / 3.0f, center.Y + BLOCK_LENGTH / 3.0f }, Vector2{ 10.0f, 180.0f });
                        }
                        s_Data.LastPos = Vector2{10.0f, 180.0f};

                        break;
                    }
                }
                while (s_Data.Board[s_Data.ScanPos.X + 1][s_Data.ScanPos.Y + 1] != 0);

                commandBuffer.Move(s_Data.LastPos, {120.0f + BLOCK_LENGTH * s_Data.ScanPos.X, 100.0f + BLOCK_LENGTH * s_Data.ScanPos.Y - CAMERA_OFFSET});
                s_Data.LastPos = {120.0f + BLOCK_LENGTH * s_Data.ScanPos.X, 100.0f + BLOCK_LENGTH * s_Data.ScanPos.Y};
            }

            writingMachine.OnUpdate(dt);
            break;
        }
        case TicTacToeData::State::AITurn:
        {
            writingMachine.OnUpdate(dt);

            if (writingMachine.Finished())
                s_Data.GameState = TicTacToeData::State::PlayerTurn;
            break;
        }
        case TicTacToeData::State::GameOver:
        {
            break;
        }
    }
}

void TicTacToeState::OnExit()
{
    ESP32Program::Get().GetWritingMachine().GetCommandBuffer().Clear();
}

void TicTacToeState::CaptureAndSendPhoto()
{
    camera_fb_t* fb = Camera::Capture();

    if (fb == nullptr)
    {
        IO::DisplayMessage(3, "Failed to take photo");
        return;
    }
    else
    {
        IO::DisplayMessage(3, "Photo taken");
    }

    // Send photo
    uint8_t header[5];
    header[0] = CV_HEADER_BYTE;
    
    // Width and height (big endian)
    header[1] = fb->width >> 8;
    header[2] = fb->width & 0xFF;
    header[3] = fb->height >> 8;
    header[4] = fb->height & 0xFF;
    
    WifiInterface::SendBytesToClient(header, 5);
    WifiInterface::SendBytesToClient((uint8_t*)fb->buf, fb->len);
}

int TicTacToeState::WaitAndAcceptForResponse()
{
    uint8_t response;
    WifiInterface::ReadBytesFromClient(&response, 1);

    if (response == 0) // Blank
    {
        IO::DisplayMessage(3, "Blank Detected");
    }
    else if (response == 1) // Cross
    {
        IO::DisplayMessage(3, "Cross Detected");
        if (s_Data.IsUserCross == -1)
            s_Data.IsUserCross = 1;

        s_Data.Board[s_Data.ScanPos.X + 1][s_Data.ScanPos.Y + 1] = s_Data.IsUserCross ? 1 : 2;
    }
    else if (response == 2) // Circle
    {
        IO::DisplayMessage(3, "Circle Detected");
        if (s_Data.IsUserCross == -1)
            s_Data.IsUserCross = 0;

        s_Data.Board[s_Data.ScanPos.X + 1][s_Data.ScanPos.Y + 1] = s_Data.IsUserCross ? 2 : 1;
    }

    return s_Data.Board[s_Data.ScanPos.X + 1][s_Data.ScanPos.Y + 1];
}

Vector2Int TicTacToeState::DecideAction()
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (s_Data.Board[i][j] == 0)
                return { i, j };
}