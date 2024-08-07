from PIL import Image
import struct

def convert_png_to_rgb565(png_path, output_path):
    # 打开PNG图像
    img = Image.open(png_path)
    img = img.convert('RGB')  # 确保图像是RGB格式

    # 获取图像尺寸
    width, height = img.size

    # 创建一个字节数组来存储RGB565数据
    rgb565_data = bytearray()

    # 遍历图像的每个像素
    for y in range(height):
        for x in range(width):
            r, g, b = img.getpixel((x, y))
            # 将RGB888转换为RGB565
            rgb565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)
            # 将RGB565数据添加到字节数组中
            rgb565_data += struct.pack('H', rgb565)

    # 将RGB565数据写入输出文件
    with open(output_path, 'wb') as f:
        f.write(rgb565_data)

    print(f"转换完成：{output_path}")

# 示例用法
convert_png_to_rgb565('input.png', 'output.rgb565')