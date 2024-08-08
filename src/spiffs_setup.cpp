#include <Arduino.h>
#include <SPIFFS.h>

void initSPIFFS() {
    // 初始化SPIFFS
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS Mount Failed");
        return;
    }

    // 写入HTML文件内容到SPIFFS
    File file = SPIFFS.open("/index.html", FILE_WRITE);
    if (!file) {
        Serial.println("Failed to open file for writing");
        return;
    }

    const char* htmlContent = R"rawliteral(
<!DOCTYPE html>
<html>

<head>
    <title>Performance Monitor</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f0f0f0;
            margin: 0;
            padding: 0;
        }

        .navbar {
            background-color: #333;
            overflow: hidden;
        }

        .navbar a {
            float: left;
            display: block;
            color: #f2f2f2;
            text-align: center;
            padding: 14px 16px;
            text-decoration: none;
        }

        .navbar a:hover {
            background-color: #ddd;
            color: black;
        }

        .container {
            max-width: 800px;
            margin: 50px auto;
            padding: 20px;
            background-color: #fff;
            border-radius: 10px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
        }

        .label {
            font-weight: bold;
        }

        .chart-container {
            width: 100%;
            height: 400px;
            margin-top: 20px;
        }

        footer {
            text-align: center;
            padding: 10px;
            background-color: #333;
            color: #f2f2f2;
            position: fixed;
            width: 100%;
            bottom: 0;
        }

        .upload-form {
            margin-top: 20px;
        }

        .upload-form h2 {
            margin-top: 0;
        }

        .upload-form form {
            margin-bottom: 20px;
            background-color: #f9f9f9;
            padding: 20px;
            border-radius: 10px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
        }

        .upload-form textarea,
        .upload-form input[type="file"] {
            width: 100%;
            padding: 10px;
            margin: 10px 0;
            border: 1px solid #ccc;
            border-radius: 5px;
        }

        .upload-form button {
            background-color: #333;
            color: #f2f2f2;
            padding: 10px 20px;
            border: none;
            border-radius: 5px;
            cursor: pointer;
        }

        .upload-form button:hover {
            background-color: #555;
        }

        .gyro-data {
            margin-top: 20px;
        }

        .gyro-data h2 {
            margin-top: 0;
        }

        .gyro-data p {
            margin: 5px 0;
        }
    </style>
    <script src='https://cdn.jsdelivr.net/npm/chart.js'></script>
</head>

<body>
    <div class='navbar'>
        <a href='#home'>Home</a>
        <a href='#performance'>Performance</a>
        <a href='#about'>About</a>
    </div>
    <div class='container'>
        <h1>Performance Monitor</h1>
        <p><span class='label'>Heap Memory:</span> <span id='heap'>Loading...</span> bytes</p>
        <p><span class='label'>CPU Frequency:</span> <span id='cpu'>Loading...</span> MHz</p>
        <p><span class='label'>WiFi Signal Strength:</span> <span id='wifi'>Loading...</span> dBm</p>
        <p><span class='label'>Uptime:</span> <span id='uptime'>Loading...</span></p>
        <div class='chart-container'>
            <canvas id='performanceChart'></canvas>
        </div>
        <div class='upload-form'>
            <h2>Upload Text</h2>
            <form id="textUploadForm">
                <textarea id="textInput" rows="4" cols="50" placeholder="Enter text here..."></textarea><br>
                <button type="button" onclick="uploadText()">Upload Text</button>
            </form>
            <h2>Upload PNG Image</h2>
            <form id="imageUploadForm" enctype="multipart/form-data">
                <input type="file" id="imageInput" accept="image/png"><br>
                <button type="button" onclick="uploadImage()">Upload Image</button>
            </form>
        </div>
        <div class='gyro-data'>
            <h2>Gyro Data</h2>
            <div class='chart-container'>
                <canvas id='gyroChart'></canvas>
            </div>
        </div>
    </div>
    <footer>&copy; 2023 Performance Monitor. All rights reserved.</footer>
    <script>
        let heapData = [];
        let cpuData = [];
        let labels = [];
        const ctx = document.getElementById('performanceChart').getContext('2d');
        const performanceChart = new Chart(ctx, {
            type: 'line',
            data: {
                labels: labels,
                datasets: [
                    {
                        label: 'Heap Memory',
                        data: heapData,
                        borderColor: 'rgba(75, 192, 192, 1)',
                        borderWidth: 1,
                        fill: false
                    },
                    {
                        label: 'CPU Frequency',
                        data: cpuData,
                        borderColor: 'rgba(153, 102, 255, 1)',
                        borderWidth: 1,
                        fill: false
                    }
                ]
            },
            options: {
                scales: {
                    x: {
                        display: true,
                        title: {
                            display: true,
                            text: 'Time'
                        }
                    },
                    y: {
                        display: true,
                        title: {
                            display: true,
                            text: 'Value'
                        }
                    }
                }
            }
        });

        function updatePerformance() {
            fetch('/performance').then(response => response.json()).then(data => {
                document.getElementById('heap').textContent = data.heap;
                document.getElementById('cpu').textContent = data.cpu;
                document.getElementById('wifi').textContent = data.wifi;
                document.getElementById('uptime').textContent = data.uptime;
                const currentTime = new Date().toLocaleTimeString();
                labels.push(currentTime);
                heapData.push(data.heap);
                cpuData.push(data.cpu);
                if (labels.length > 20) {
                    labels.shift();
                    heapData.shift();
                    cpuData.shift();
                }
                performanceChart.update();
            }).catch(error => console.error('Error fetching performance data:', error));
        }

        setInterval(updatePerformance, 1000);
        updatePerformance();

        function uploadText() {
            const text = document.getElementById('textInput').value;
            fetch('/upload', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded'
                },
                body: 'text=' + encodeURIComponent(text)
            }).then(response => response.text()).then(data => {
                alert(data);
            }).catch(error => console.error('Error uploading text:', error));
        }

        function uploadImage() {
            const imageInput = document.getElementById('imageInput');
            const formData = new FormData();
            formData.append('image', imageInput.files[0]);
            fetch('/upload_image', {
                method: 'POST',
                body: formData
            }).then(response => response.text()).then(data => {
                alert(data);
            }).catch(error => console.error('Error uploading image:', error));
        }

        let gyroDataX = [];
        let gyroDataY = [];
        let gyroDataZ = [];
        let gyroLabels = [];
        const gyroCtx = document.getElementById('gyroChart').getContext('2d');
        const gyroChart = new Chart(gyroCtx, {
            type: 'line',
            data: {
                labels: gyroLabels,
                datasets: [
                    {
                        label: 'GX',
                        data: gyroDataX,
                        borderColor: 'rgba(255, 99, 132, 1)',
                        borderWidth: 1,
                        fill: false
                    },
                    {
                        label: 'GY',
                        data: gyroDataY,
                        borderColor: 'rgba(54, 162, 235, 1)',
                        borderWidth: 1,
                        fill: false
                    },
                    {
                        label: 'GZ',
                        data: gyroDataZ,
                        borderColor: 'rgba(75, 192, 192, 1)',
                        borderWidth: 1,
                        fill: false
                    }
                ]
            },
            options: {
                scales: {
                    x: {
                        display: true,
                        title: {
                            display: true,
                            text: 'Time'
                        }
                    },
                    y: {
                        display: true,
                        title: {
                            display: true,
                            text: 'Value'
                        }
                    }
                }
            }
        });

        async function fetchGyroData() {
            const response = await fetch('/gyro');
            const data = await response.json();
            const currentTime = new Date().toLocaleTimeString();
            gyroLabels.push(currentTime);
            gyroDataX.push(data.gx);
            gyroDataY.push(data.gy);
            gyroDataZ.push(data.gz);
            if (gyroLabels.length > 20) {
                gyroLabels.shift();
                gyroDataX.shift();
                gyroDataY.shift();
                gyroDataZ.shift();
            }
            gyroChart.update();
        }

        setInterval(fetchGyroData, 1000);
    </script>
</body>

</html>
)rawliteral";

    file.print(htmlContent);
    file.close();
}