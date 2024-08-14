import cv2 as cv
import sys
import numpy as np
import os

"""
What i need to do:
open image
get rgb values
transform rgb values to luminance values; save both values
display the rgb:
	"[(R,G,B)+(R,G,B)+...]
	 [...]"
convolution in:
	binary - 1 point to 6 point
	distance - 1 point to 6 point
	edge - 1 point to 6 point
"""

imgs = []
high_change = [0,0,255]
mid_change = [0,255,255]
low_change = [255,127,0]
LUMDELTApath = "C:/Users/mpidi/Desktop/WORKSPACE/LUMDELTAheatmapimgs/"
RGBDELTApath = "C:/Users/mpidi/Desktop/WORKSPACE/RGBDELTAheatmapimgs/"
LUMpath = "C:/Users/mpidi/Desktop/WORKSPACE/LUMheatmapimgs/"
DSETSpath = "C:/Users/mpidi/Desktop/WORKSPACE/IMGDSET/"
list_of_dirs = os.listdir(RGBDELTApath)  
def bin_asses_EDGE_heatmap_RGBDELTA_color_change():
    for i in range(len(imgs)):
        csv_to_output = open("C:/Users/mpidi/Desktop/WORKSPACE/BIN_DATA/RGBDELTAdata/bin_EDGE_RGBDELTA_Class_"+list_of_dirs[i]+".csv",'w')
        string_to_write = "Conv0, Conv1, Conv2, Conv3, Conv4, Conv5, Conv6, Conv7, Conv8, Conv9\n"
        value_to_write = ""
        for b in range(228,80,-1):
            value_to_write += "1" if imgs[i][b][159].tolist() == high_change else "0"
            #avg_color_row_1 = np.mean(imgs[i][10:11][159:160],axis=0,dtype=np.int32)
            #if (avg_color_row_1) :
            #    img_to_create[j].append(high_change)
            temp_imgs = imgs[i].tolist()
            for j in range(1,10):
                countR = 0
                countY = 0
                countB = 0
                for k in temp_imgs[b-j:b+1+j]:
                    for l in k[159-j:160+j]:
                        countR += 1 if l == high_change else 0
                        countY += 1 if l == mid_change else 0
                        countB += 1 if l == low_change else 0
                if (countR > countY and countR > countB):
                    value_to_write += ", 1"
                else:
                    value_to_write += ", 0"
            value_to_write += f", PIXEL #{b}\n"
        string_to_write += value_to_write
        csv_to_output.write(string_to_write)
        csv_to_output.close()
def bin_assess_EDGE_heatmap_LUMDELTA_color_change():
    for i in range(len(imgs)):
        csv_to_output = open("C:/Users/mpidi/Desktop/WORKSPACE/BIN_DATA/LUMDELTAdata/bin_EDGE_LUMDELTA_Class_"+list_of_dirs[i]+".csv",'w')
        string_to_write = "Conv0, Conv1, Conv2, Conv3, Conv4, Conv5, Conv6, Conv7, Conv8, Conv9\n"
        value_to_write = ""
        for b in range(228,80,-1):
            value_to_write += "1" if imgs[i][b][159].tolist() == high_change else "0"
            #avg_color_row_1 = np.mean(imgs[i][10:11][159:160],axis=0,dtype=np.int32)
            #if (avg_color_row_1) :
            #    img_to_create[j].append(high_change)
            temp_imgs = imgs[i].tolist()
            for j in range(1,10):
                countR = 0
                countY = 0
                countB = 0
                for k in temp_imgs[b-j:b+1+j]:
                    for l in k[159-j:160+j]:
                        countR += 1 if l == high_change else 0
                        countY += 1 if l == mid_change else 0
                        countB += 1 if l == low_change else 0
                if (countR > countY and countR > countB):
                    value_to_write += ", 1"
                else:
                    value_to_write += ", 0"
            value_to_write += f", PIXEL #{b}\n"
        string_to_write += value_to_write
        csv_to_output.write(string_to_write)
        csv_to_output.close()
def bin_asses_LUM_heatmap_of_color_change():
    csv_to_output = open("C:/Users/mpidi/Desktop/WORKSPACE/BIN_DATA/bin_Class.csv",'w')
    string_to_write = "Conv0, Conv1, Conv2, Conv3, Conv4, Conv5, Conv6, Conv7, Conv8, Conv9\n"
    for i in range(len(imgs)):
        
        value_to_write = "1" if imgs[i][189][159].tolist() == high_change else "0"
        #avg_color_row_1 = np.mean(imgs[i][10:11][159:160],axis=0,dtype=np.int32)
        #if (avg_color_row_1) :
        #    img_to_create[j].append(high_change)
        temp_imgs = imgs[i].tolist()
        for j in range(1,10):
            countR = 0
            countY = 0
            countB = 0
            for k in temp_imgs[189-j:190+j]:
                for l in k[159-j:160+j]:
                    countR += 1 if l == high_change else 0
                    countY += 1 if l == mid_change else 0
                    countB += 1 if l == low_change else 0
            
            if (countR > countY and countR > countB):
                value_to_write += ", 1"
            else:
                value_to_write += ", 0"
        value_to_write += ", "+list_of_dirs[i]+"\n"
        string_to_write += value_to_write
    csv_to_output.write(string_to_write)
    csv_to_output.close()
def create_LUM_heatmap_of_color_change():
    img_to_create = []
    for i in range(len(imgs)):
        img_to_create = []
        for j in range(len(imgs[i])):
            img_to_create.append([])
            for k in range(0,320):
                avg_color_row_1 = np.mean(imgs[i][j][k:k+1],axis=0,dtype=np.int32)*[.07,.7,.22]
                if sum(avg_color_row_1) > 110.:
                    img_to_create[j].append(high_change)
                elif sum(avg_color_row_1) > 90.:
                    img_to_create[j].append(mid_change)
                else:
                    img_to_create[j].append(low_change)
        cv.imwrite("C:/Users/mpidi/Desktop/LUMheatmapimgs/heatmap_"+list_of_dirs[i],np.array(img_to_create))
def create_LUMDELTA_heatmap_of_color_change():
    img_to_create = []
    for i in range(len(imgs)):
        img_to_create = []
        for j in range(len(imgs[i])-1):
            img_to_create.append([])
            for k in range(0,320):
                avg_color_row_1 = np.mean(imgs[i][j][k:k+1],axis=0,dtype=np.int32)*[.07,.7,.22]
                avg_color_row_2 = np.mean(imgs[i][j+1][k:k+1],axis=0,dtype=np.int32)*[.07,.7,.22]
                delta = np.abs(sum(avg_color_row_1) - sum(avg_color_row_2))
                if (delta) > 7.:
                    img_to_create[j].append(high_change)
                elif (delta) > 5.:
                    img_to_create[j].append(mid_change)
                else:
                    img_to_create[j].append(low_change)
        cv.imwrite("C:/Users/mpidi/Desktop/LUMDELTAheatmapimgs/heatmap_"+list_of_dirs[i],np.array(img_to_create))
def create_RGBDELTA_heatmap_of_color_change():
    img_to_create = []
    for i in range(len(imgs)):
        img_to_create = []
        for j in range(len(imgs[i])-1):
            img_to_create.append([])
            for k in range(0,320):
                avg_color_row_1 = np.mean(imgs[i][j][k:k+1],axis=0,dtype=np.int32)
                avg_color_row_2 = np.mean(imgs[i][j+1][k:k+1],axis=0,dtype=np.int32)
                delta = np.abs(avg_color_row_1 - avg_color_row_2)
                if sum(delta) > 18.:
                    img_to_create[j].append(high_change)
                elif sum(delta) > 9.:
                    img_to_create[j].append(mid_change)
                else:
                    img_to_create[j].append(low_change)
        cv.imwrite("C:/Users/mpidi/Desktop/RGBDELTAheatmapimgs/heatmap_"+list_of_dirs[i],np.array(img_to_create))
def create_list_of_imgs(path):
    for i in list_of_dirs:
        imgs.append(cv.imread(path+i))
def display_rgb():
	"""
	What i need to do:
	
	"""
	r = 0
	g = 0
	b = 0

	output_str = "*"
	for i in range(120,240):
		for j in range(159,160):
			for k in range(8):
				output_str += f"+{imgs[k][i][j]}\t"
		output_str += "*\n*"
	print(output_str)
    

create_list_of_imgs(RGBDELTApath)
"""for i in range(1):
    for j in range(3):
        avg_color_row_1 = np.mean(imgs[i][j],axis=0,dtype=np.int32)
        avg_color_row_2 = np.mean(imgs[i][j+1],axis=0,dtype=np.int32)
        delta = abs(avg_color_row_1 - avg_color_row_2)
        if i == 0:
            print("the sum:")
            print(np.mean(imgs[i][j],axis=0,dtype=np.int32))
            print("*"*20)
            print(avg_color_row_1)
            print("*"*20)
            print(avg_color_row_2)
            print("*"*20)
            print(delta)"""
#create_LUM_heatmap_of_color_change()
#display_rgb()
"""
On the LUMpath
Start @P(10,159) and do conv2d:
    radius = 1
    radius = 2
    radius = 3
    radius = 4
    radius = 5
    radius = 6
    radius = 7
    radius = 8
    radius = 9
    radius = 10
Ask the question "is it red?"
    If it is red -> it is a wall
    Else it is not a wall
"""
#bin_asses_LUM_heatmap_of_color_change()
#print(imgs[0][10][159])
#bin_assess_EDGE_heatmap_LUMDELTA_color_change()
#bin_asses_EDGE_heatmap_RGBDELTA_color_change()