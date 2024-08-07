import requests

# ESP32服务器的IP地址和端点
url = "http://192.168.2.7/upload_image"

# 要上传的图片文件路径
image_path = "E:\School\R\Robocon\hardware\esp32\code\iBOX\input.png"

# 打开图片文件并发送POST请求
with open(image_path, 'rb') as image_file:
    files = {'file': image_file}
    response = requests.post(url, files=files)

# 打印服务器响应
print(f"Status Code: {response.status_code}")
print(f"Response Text: {response.text}")