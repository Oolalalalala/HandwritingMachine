import os
from ttfquery import describe
from ttfquery import glyphquery
import ttfquery.glyph as glyph

font_dir = os.path.join(os.getcwd(), "Python", "fonts", "Arial.ttf")
font = describe.openFont(font_dir)
glyf = font['glyf']
print(font)
print(glyf)