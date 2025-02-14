from PIL import Image


colorPalette = []
darkPalette = [(-1,-1,-1)]

spriteSheet = []

darkColor = (31, 16, 70)
alpha = 0.6


with open('data/palettes/color_palette.txt', 'r') as colorFile:
	colorPalette = [tuple(map(int, line.rstrip().split(' '))) for line in colorFile]


for i in range(1, len(colorPalette)):
	pixel = colorPalette[i]
	newR = int(pixel[0] + (darkColor[0] - pixel[0]) * alpha)
	newG = int(pixel[1] + (darkColor[1] - pixel[1]) * alpha)
	newB = int(pixel[2] + (darkColor[2] - pixel[2]) * alpha)

	darkPalette.append((newR, newG, newB))


with open('data/palettes/dark_palette.txt', 'w') as darkFile:
	for i in range(len(colorPalette)):
		pixel = darkPalette[i]
		darkFile.write(f"{pixel[0]}, {pixel[1]}, {pixel[2]}\n")



with open('data/sheets/sprite_sheet.txt', 'r') as spriteFile:
	spriteSheetTmp = [list(map(int, line.rstrip().split(' '))) for line in spriteFile]
for i in range(len(spriteSheetTmp)):
	for j in range(16):
		spriteSheet.append(spriteSheetTmp[i][j*16:j*16+16])



img = Image.new('RGBA', (len(spriteSheet), 16), color=(0,0,0,0))
pix = img.load()
for i in range(len(spriteSheet)):
	for j in range(16):
		idx = spriteSheet[i][j]
		a = 255
		if idx == 0:
			a = 0
		pixel = (darkPalette[idx][0], darkPalette[idx][1], darkPalette[idx][2], a)

		y = i % 16
		x = (i//16)*16 + j
		pix[x,y] = pixel
img.save('data/test/dark_sheet.png')


img = Image.new('RGBA', (len(spriteSheet), 16), color=(0,0,0,0))
pix = img.load()
for i in range(len(spriteSheet)):
	for j in range(16):
		idx = spriteSheet[i][j]
		a = 255
		if idx == 0:
			a = 0
		pixel = (colorPalette[idx][0], colorPalette[idx][1], colorPalette[idx][2], a)

		y = i % 16
		x = (i//16)*16 + j
		pix[x,y] = pixel
img.save('data/test/sprite_sheet.png')

