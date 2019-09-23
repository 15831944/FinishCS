// scaleimage.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "gdal_priv.h"
#include "gdal.h"
#include <string>

std::string getFilePath(const std::string &pathname)
{
	int n = pathname.find_last_of('/');
	int m = pathname.find_last_of('\\');
	int c = m > n ? m + 1 : n + 1;
	std::string str = pathname.substr(0, c);
	return str;
}

std::string getFileName(const std::string &pathname)
{
	int n = pathname.find_last_of('/');
	int m = pathname.find_last_of('\\');
	int c = m > n ? m + 1 : n + 1;
	std::string str = pathname.substr(c, pathname.size() - c);
	return str;
}

std::string getFileTitle(const std::string &pathname)
{
	std::string filename = getFileName(pathname);
	int n = filename.find_last_of('.');
	std::string str = filename.substr(0, n);
	return str;
}

std::string getFileExt(const std::string &pathname)
{
	std::string filename = getFileName(pathname);
	int n = filename.find_last_of('.') + 1;
	std::string str;
	if (n)
	{
		str = filename.substr(n, filename.size() - n);
	}
	else
	{
		str = "";
	}
	return str;
}

int rescale_image_jpeg(const char* in_file_name, 
	const char* out_file_name, 
	const unsigned char &background_r, 
	const unsigned char &background_g,
	const unsigned char &background_b,
	const long &rescale_width, 
	const long &rescale_height,
	void *callback = NULL)
{
	GDALAllRegister();
	GDALDriverManager *driver_manager = GetGDALDriverManager();
	////////////////获取输出Dirver///////////////////////////////
	std::string in_ext = getFileExt(out_file_name);
	GDALDriver *out_driver;
	if (!_stricmp(in_ext.c_str(), "jpg"))
	{
		out_driver = driver_manager->GetDriverByName("JPEG");
	}
	else if (!_stricmp(in_ext.c_str(), "png"))
	{
		out_driver = driver_manager->GetDriverByName("PNG");
	}

	///////////获取MEM Driver////////////////////////////////
	GDALDriver *mem_driver = NULL;
	mem_driver = driver_manager->GetDriverByName("MEM");
	if (NULL == mem_driver || NULL == out_driver) 
	{ 
		return false; 
	}

	///////////////////////////打开输入影像////////////////////////////////////////
	GDALDataset *in_dataset = (GDALDataset *)GDALOpen(in_file_name, GA_ReadOnly);
	if (!in_dataset)
	{
		return 1;
	}

	int channel_count = in_dataset->GetRasterCount();
	int x_size = in_dataset->GetRasterXSize();
	int y_size = in_dataset->GetRasterYSize();

	double scale_x = (double)x_size / rescale_width;
	double scale_y = (double)y_size / rescale_height;

	int read_x = scale_x;
	int read_y = scale_y;
	
	unsigned char *in_data_space = new unsigned char[x_size*read_y*3];
	unsigned char *out_data_space = new unsigned char[rescale_width*rescale_height*3];

	double x_counter = 0;
	double y_counter = 0;

	int out_counter = 0;

	unsigned char *data_start_h;
	unsigned char *data_start_w;

	int band_map[3];
	band_map[0] = 1;
	band_map[1] = 2;
	band_map[2] = 3;

	GDALDataset *mem_dataset = mem_driver->Create("", rescale_width, rescale_height, 3, GDT_Byte, NULL);

	for (int i = 0; i < rescale_height; i++)
	{
		x_counter = 0;
		CPLErr read_error = in_dataset->RasterIO(GF_Read, 0, (int)(y_counter), x_size, read_y, in_data_space, x_size, read_y, GDT_Byte, 3, band_map, 3, x_size * 3, 1);
		data_start_h = in_data_space;
		data_start_w = data_start_h;
		for (int s = 0; s < rescale_width; s++)
		{
			out_data_space[out_counter] = data_start_w[0];
			out_data_space[out_counter + 1] = data_start_w[1];
			out_data_space[out_counter + 2] = data_start_w[2];

			for (int t = 0; t < read_y; t++)
			{
				for (int u = 0; u < read_x; u++)
				{
					if (background_r != data_start_w[u * 3] || background_g != data_start_w[u * 3 + 1] || background_b != data_start_w[u * 3 + 2])
					{
						out_data_space[out_counter] = data_start_w[u * 3];
						out_data_space[out_counter+1] = data_start_w[u * 3 + 1];
						out_data_space[out_counter+2] = data_start_w[u * 3 + 2];
					}
				}
				data_start_w += x_size * 3;
			}
			x_counter += scale_x;
			data_start_w = data_start_h + (int)(x_counter)* 3;
			out_counter += 3;
		}
		y_counter += scale_y;
		
	}

	CPLErr write_error = mem_dataset->RasterIO(GF_Write, 0, 0, rescale_width, rescale_height, out_data_space, rescale_width, rescale_height, GDT_Byte, 3, band_map, 3, rescale_width * 3, 1);

	out_driver->CreateCopy(out_file_name, mem_dataset, true, 0, 0, 0);	
}

int _tmain(int argc, _TCHAR* argv[])
{
	rescale_image_jpeg("F:/Project_iData/MainWorking/branch1.3/code/other/test_origin.PNG", "F:/Project_iData/MainWorking/branch1.3/code/other/test_result.PNG", 255, 255, 255, 1260, 1782, NULL);
	return 0;
}

