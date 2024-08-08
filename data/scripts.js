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
    const response = await fetch('/IMU');
    const data = await response.json();
    const currentTime = new Date().toLocaleTimeString();
    gyroLabels.push(currentTime);
    gyroDataX.push(data.gx);
    gyroDataY.push(data.gy);
    gyroDataZ.push(data.gz);
    document.getElementById('temp').textContent = data.temp; // 显示温度
    if (gyroLabels.length > 20) {
        gyroLabels.shift();
        gyroDataX.shift();
        gyroDataY.shift();
        gyroDataZ.shift();
    }
    gyroChart.update();
}

setInterval(fetchGyroData, 1000); // 每秒更新一次