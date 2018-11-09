from PIL import Image

def convertToTxt():
	fileName = input("Enter file name:")
	im = Image.open(fileName)
	sep = fileName.split(".")
	file = open(sep[0] + ".txt", "w")
	pix = im.load()

	file.write(str(im.size[1]) + "\n" + str(im.size[0]) + "\n")

	for row in range(im.size[1]):
		for col in range(im.size[0]):
			file.write(
			  str(pix[col, row][0]) + " " 
			+ str(pix[col, row][1]) + " " 
			+ str(pix[col, row][2]) + "\n")

	print("Image saved as " + file.name)

			
again = True

while again:
        convertToTxt()
        again = (input("Again? (Y/N):").lower() != "n")
