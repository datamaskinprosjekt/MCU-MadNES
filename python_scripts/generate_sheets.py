from PIL import Image


colorPalette = [(-1,-1,-1)]
spriteSheet = []


def add_pixel(pixel):
	if not pixel[3]:
		return 0
	pixel = pixel[:3]
	for i in range(len(colorPalette)):
		if pixel == colorPalette[i]:
			return i
	colorPalette.append(pixel)
	return len(colorPalette) - 1


def add_sprite(spriteSheet, filename):
	im = Image.open(filename)
	pix = im.load()
	width, height = im.size
	spriteSize = width // 16

	sprites = [[0 for _ in range(16)] for _ in range(spriteSize * 16)]
	#print(f"{filename} width={width} height={height} size={spriteSize}")

	for spriteIdx in range(spriteSize):
		for i in range(16):
			for j in range(16):
				pixel = pix[spriteIdx * 16 + j, i]
				colorIdx = add_pixel(pixel)
				sprites[spriteIdx * 16 + i][j] = colorIdx
	spriteSheet += sprites


def save_sheets():
	with open('sheets/color_palette.txt', 'w') as colorFile:
		for i in range(len(colorPalette)):
			pixel = colorPalette[i]
			colorFile.write(f"{pixel[0]} {pixel[1]} {pixel[2]}\n")

	with open('sheets/sprite_sheet.txt', 'w') as sheetFile:
		for i in range(len(spriteSheet)):
			s = ""
			for j in range(16):
				s += str(spriteSheet[i][j]) + " "
			sheetFile.write(s)
			if (i+1) % 16 == 0:
				s = s.rstrip()
				sheetFile.write("\n")


def print_sheets():
	for i in range(len(colorPalette)):
		pixel = colorPalette[i]
		print(f"{i}: {pixel}")

	for i in range(len(spriteSheet)):
		if i % 16 == 0:
			print()
		for j in range(16):
			print(spriteSheet[i][j], end=" ");
		print()


add_sprite(spriteSheet, 'img/ship16-3.png')
add_sprite(spriteSheet, 'img/ship16-4.png')
add_sprite(spriteSheet, 'img/ship16-2.png')
add_sprite(spriteSheet, 'img/ship16-1.png')
add_sprite(spriteSheet, 'img/ship16-8.png')
add_sprite(spriteSheet, 'img/ship16-9.png')
add_sprite(spriteSheet, 'img/ship16-5.png')
add_sprite(spriteSheet, 'img/ship16-6.png')
add_sprite(spriteSheet, 'img/ship16-7.png')
add_sprite(spriteSheet, 'img/status-1.png')
add_sprite(spriteSheet, 'img/status-2.png')
add_sprite(spriteSheet, 'img/status-3.png')
add_sprite(spriteSheet, 'img/status-4.png')
add_sprite(spriteSheet, 'img/status-5.png')
add_sprite(spriteSheet, 'img/status-6.png')
add_sprite(spriteSheet, 'img/status-7.png')
add_sprite(spriteSheet, 'img/status-8.png')
add_sprite(spriteSheet, 'img/status-9.png')
add_sprite(spriteSheet, 'img/asteroid-1.png')
add_sprite(spriteSheet, 'img/asteroid-2.png')
add_sprite(spriteSheet, 'img/asteroid-3.png')
add_sprite(spriteSheet, 'img/laser-2.png')
add_sprite(spriteSheet, 'img/gameover-1.png')
add_sprite(spriteSheet, 'img/start-1.png')
add_sprite(spriteSheet, 'img/stars-1.png')

save_sheets()


