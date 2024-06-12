import os
from ttfquery import describe
from ttfquery import glyphquery
import ttfquery.glyph as glyph

font_dir = os.path.join(os.getcwd(), "Python", "fonts", "Consolas.ttf")
font = describe.openFont(font_dir)
g = glyph.Glyph(glyphquery.glyphName(font, "M"))
width = glyphquery.width(font, g.glyphName)
height = glyphquery.lineHeight(font)
print(height)