import random
from PIL import Image, ImageDraw, ImageFont
import recognition as rg
import math as m

width = 1000 #int(input('input width:')) стандарт тыща но можно вводить если поставить хуйню в коментах на место тыщи
height = 1000 #int(input('input height: ')) тоже самое но высота
font = ImageFont.truetype("C:\\Windows\\Fonts\\Arial.ttf", 90)
#font_num = ImageFont.truetype("C:\\Windows\\Fonts\\Arial.ttf", 30)
img = Image.new("RGB", (height, width), "white")
draw = ImageDraw.Draw(img)

def draw_p(char, cord):
	draw.text((cord.x - (int)(font.getsize(char)[0]/2), cord.y - (int)(font.getsize(char)[1]/2)), char, fill='black', font=font)

def draw_pp(char, cord):
	draw.text((cord.x, cord.y), char, fill='black', font=font)

def drow_V(cord1, cord2, type):
	draw = ImageDraw.Draw(img)
	if (type == 1):
		draw.line((cord1.x, cord1.y, cord2.x, cord2.y), fill="black", width=4)
	elif (type == 2):
		dy = cord2.y - cord1.y
		dx = cord2.x - cord1.x
		if dy >= 0 and dx >= 0:
			draw.line((cord1.x, cord1.y, cord2.x, cord2.y), fill="black", width=4);
			s = [(cord2.x + cord1.x)/2, (cord2.y + cord1.y)/2];
			n = [cord2.y - cord1.y, - cord2.x + cord1.x];
			nap = [cord2.x - cord1.x, cord2.y - cord1.y];
			draw.line((s[0] + n[0]/12 - nap[0]/5, s[1] + n[1]/12 - nap[1]/5, s[0] + n[0]/12 + nap[0]/5, s[1] + n[1]/12 + nap[1]/5), fill="black", width=4);
		elif dy >= 0 and dx <= 0:
			draw.line((cord1.x, cord1.y, cord2.x, cord2.y), fill="black", width=4);
			s = [(cord2.x + cord1.x)/2, (cord2.y + cord1.y)/2];
			n = [-(cord2.y - cord1.y), -(-cord2.x + cord1.x)];
			nap = [-(cord2.x - cord1.x), -(cord2.y - cord1.y)];
			draw.line((s[0] + n[0]/12 - nap[0]/5, s[1] + n[1]/12 - nap[1]/5, s[0] + n[0]/12 + nap[0]/5, s[1] + n[1]/12 + nap[1]/5), fill="black", width=4);
		elif dy <= 0 and dx >= 0:
			draw.line((cord1.x, cord1.y, cord2.x, cord2.y), fill="black", width=4);
			s = [(cord2.x + cord1.x)/2, (cord2.y + cord1.y)/2];
			n = [-(cord2.y - cord1.y), -(-cord2.x + cord1.x)];
			nap = [-(cord2.x - cord1.x), -(cord2.y - cord1.y)];
			draw.line((s[0] + n[0]/12 - nap[0]/5, s[1] + n[1]/12 - nap[1]/5, s[0] + n[0]/12 + nap[0]/5, s[1] + n[1]/12 + nap[1]/5), fill="black", width=4);
		elif dy <= 0 and dx <= 0:
			draw.line((cord1.x, cord1.y, cord2.x, cord2.y), fill="black", width=4);
			s = [(cord2.x + cord1.x)/2, (cord2.y + cord1.y)/2];
			n = [(cord2.y - cord1.y), (-cord2.x + cord1.x)];
			nap = [(cord2.x - cord1.x), (cord2.y - cord1.y)];
			draw.line((s[0] + n[0]/12 - nap[0]/5, s[1] + n[1]/12 - nap[1]/5, s[0] + n[0]/12 + nap[0]/5, s[1] + n[1]/12 + nap[1]/5), fill="black", width=4);
	elif (type == 3):
		draw.line((cord1.x, cord1.y, cord2.x, cord2.y), fill="black", width=4);
		s = [(cord2.x + cord1.x)/2, (cord2.y + cord1.y)/2];
		n1 = [-(cord2.y - cord1.y), -(-cord2.x + cord1.x)]
		n = [cord2.y - cord1.y, -cord2.x + cord1.x];
		nap = [cord2.x - cord1.x, cord2.y - cord1.y];
		nap1 = [-(cord2.x - cord1.x), -(cord2.y - cord1.y)];
		draw.line(((s[0] + n1[0]/12 - nap1[0]/5), (s[1] + n1[1]/12 - nap1[1]/5), (s[0] + n1[0]/12 + nap1[0]/5), (s[1] + n1[1]/12 + nap1[1]/5)), fill="black", width=4);
		draw.line((s[0] + n[0]/12 - nap[0]/5, s[1] + n[1]/12- nap[1]/5, s[0] + n[0]/12 + nap[0]/5, s[1] + n[1]/12 + nap[1]/5), fill="black", width=4);

def build_image_impl(element):
	for i in range(len(element.elements)):
		tan = 1
		k_zero = 1
		cord_x1 = 0
		cord_x2 = 0
		cord_p1 = rg.Coord(0, 0)
		cord_p2 = rg.Coord(0, 0)
		cord_y = font.getsize("A")[1]/2
		for j in range(len(element.name)):
			cord_x1 += (int)(font.getsize(element.name[j])[0])
		for q in range(len(element.elements[i][0].name)):
			cord_x2 += (int)(font.getsize(element.elements[i][0].name[q])[0])
		cord_x1 = cord_x1/2
		cord_x2 = cord_x2/2
		dx = (element.elements[i][0].coord.x - element.coord.x)
		dy = (element.elements[i][0].coord.y - element.coord.y)
		if dx == 0:
			k_zero = 0
		if k_zero == 1:
			tan = dy/dx
		if (abs(tan) > cord_y/cord_x1):
			if (dx >= 0 and dy <= 0):
				cord_p1 = rg.Coord(element.coord.x + k_zero*int(cord_y/abs(tan)), element.coord.y - cord_y)
			elif (dx <= 0 and dy >= 0):
				cord_p1 = rg.Coord(element.coord.x - k_zero*int(cord_y/abs(tan)), element.coord.y + cord_y)
			elif (dx <= 0 and dy <= 0):
				cord_p1 = rg.Coord(element.coord.x - k_zero*int(cord_y/abs(tan)), element.coord.y - cord_y)
			elif (dx >= 0 and dy >= 0):
				cord_p1 = rg.Coord(element.coord.x + k_zero*int(cord_y/abs(tan)), element.coord.y + cord_y)
		else:
			if (dx >= 0 and dy <= 0):
				cord_p1 = rg.Coord(element.coord.x + k_zero*cord_x1, element.coord.y - int(cord_x1*abs(tan)))
			elif (dx <= 0 and dy >= 0):
				cord_p1 = rg.Coord(element.coord.x - k_zero*cord_x1, element.coord.y + int(cord_x1*abs(tan)))
			elif (dx <= 0 and dy <= 0):
				cord_p1 = rg.Coord(element.coord.x - k_zero*cord_x1, element.coord.y - int(cord_x1*abs(tan)))
			elif (dx >= 0 and dy >= 0):
				cord_p1 = rg.Coord(element.coord.x + k_zero*cord_x1, element.coord.y + int(cord_x1*abs(tan)))
		if (abs(tan) > cord_y/cord_x2):
			if (dx >= 0 and dy <= 0):
				cord_p2 = rg.Coord(element.elements[i][0].coord.x - k_zero*int(cord_y/abs(tan)), element.elements[i][0].coord.y + cord_y)
			elif (dx <= 0 and dy >= 0):
				cord_p2 = rg.Coord(element.elements[i][0].coord.x + k_zero*int(cord_y/abs(tan)), element.elements[i][0].coord.y - cord_y)
			elif (dx <= 0 and dy <= 0):
				cord_p2 = rg.Coord(element.elements[i][0].coord.x + k_zero*int(cord_y/abs(tan)), element.elements[i][0].coord.y + cord_y)
			elif (dx >= 0 and dy >= 0):
				cord_p2 = rg.Coord(element.elements[i][0].coord.x - k_zero*int(cord_y/abs(tan)), element.elements[i][0].coord.y - cord_y)
		else:
			if (dx >= 0 and dy <= 0):
				cord_p2 = rg.Coord(element.elements[i][0].coord.x - k_zero*cord_x2, element.elements[i][0].coord.y + cord_x2*abs(tan))
			elif (dx <= 0 and dy >= 0):
				cord_p2 = rg.Coord(element.elements[i][0].coord.x + k_zero*cord_x2, element.elements[i][0].coord.y - cord_x2*abs(tan))
			elif (dx <= 0 and dy <= 0):
				cord_p2 = rg.Coord(element.elements[i][0].coord.x + k_zero*cord_x2, element.elements[i][0].coord.y + cord_x2*abs(tan))
			elif (dx >= 0 and dy >= 0):
				cord_p2 = rg.Coord(element.elements[i][0].coord.x - k_zero*cord_x2, element.elements[i][0].coord.y - cord_x2*abs(tan))
		drow_V(cord_p1, cord_p2, element.elements[i][1])
		build_image_impl(element.elements[i][0])
	n = len(element.name)
	if n == 1:
		draw_p(element.name[0], element.coord)
	elif (n > 1):
		for i in range(n):
			cord_p = rg.Coord(element.coord.x - (int)((n/2 - i)*font.getsize(element.name[i])[0]) - 3*i, element.coord.y - (int)(font.getsize(element.name[i])[1]/2))
			draw_pp(element.name[i], cord_p)

def build_image(elem):
    build_image_impl(elem) #elema poka net
    img.save("draw.png", "PNG")

#--------EXAMPLE-------------
cord1 = rg.Coord(40, 500)
cord2 = rg.Coord(40+100, 500-173)
cord3 = rg.Coord(40+100+200, 500-173)
cord4 = rg.Coord(40+200+200, 500)
cord5 = rg.Coord(40+100+200, 500+173)
cord6 = rg.Coord(40+100, 500+173)
elem = rg.Element(cord1, "Р", [[rg.Element(cord2, 'H2O', [[rg.Element(cord5, 'HO2', []), 1], [rg.Element(cord6, 'HP', []), 2]]), 3], [rg.Element(cord5, 'HO2', []), 2], [rg.Element(cord3, 'CN', [[rg.Element(cord5, 'HO2', []), 2]]), 2], [rg.Element(cord4, 'NH', [[rg.Element(cord5, 'HO2', []), 2], [rg.Element(cord3, 'CN', []), 2]]), 2]])
#----------------------------
build_image(elem)
img.show()

