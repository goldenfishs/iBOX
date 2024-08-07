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
    file.print("<!DOCTYPE html><html><head><title>Performance Monitor</title><style>body { font-family: Arial, sans-serif; background-color: #f0f0f0; margin: 0; padding: 0; } .navbar { background-color: #333; overflow: hidden; } .navbar a { float: left; display: block; color: #f2f2f2; text-align: center; padding: 14px 16px; text-decoration: none; } .navbar a:hover { background-color: #ddd; color: black; } .container { max-width: 800px; margin: 50px auto; padding: 20px; background-color: #fff; border-radius: 10px; box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); } .label { font-weight: bold; } .chart-container { width: 100%; height: 400px; margin-top: 20px; } footer { text-align: center; padding: 10px; background-color: #333; color: #f2f2f2; position: fixed; width: 100%; bottom: 0; }</style><script src='https://cdn.jsdelivr.net/npm/chart.js'></script></head><body><div class='navbar'><a href='#home'>Home</a><a href='#performance'>Performance</a><a href='#about'>About</a></div><div class='container'><h1>Performance Monitor</h1><p><span class='label'>Heap Memory:</span> <span id='heap'>Loading...</span> bytes</p><p><span class='label'>CPU Frequency:</span> <span id='cpu'>Loading...</span> MHz</p><p><span class='label'>WiFi Signal Strength:</span> <span id='wifi'>Loading...</span> dBm</p><p><span class='label'>Uptime:</span> <span id='uptime'>Loading...</span></p><div class='chart-container'><canvas id='performanceChart'></canvas></div></div><footer>© 2023 Performance Monitor. All rights reserved.</footer><script>let heapData = []; let cpuData = []; let labels = []; const ctx = document.getElementById('performanceChart').getContext('2d'); const performanceChart = new Chart(ctx, { type: 'line', data: { labels: labels, datasets: [ { label: 'Heap Memory', data: heapData, borderColor: 'rgba(75, 192, 192, 1)', borderWidth: 1, fill: false }, { label: 'CPU Frequency', data: cpuData, borderColor: 'rgba(153, 102, 255, 1)', borderWidth: 1, fill: false } ] }, options: { scales: { x: { display: true, title: { display: true, text: 'Time' } }, y: { display: true, title: { display: true, text: 'Value' } } } } }); function updatePerformance() { fetch('/performance').then(response => response.json()).then(data => { document.getElementById('heap').textContent = data.heap; document.getElementById('cpu').textContent = data.cpu; document.getElementById('wifi').textContent = data.wifi; document.getElementById('uptime').textContent = data.uptime; const currentTime = new Date().toLocaleTimeString(); labels.push(currentTime); heapData.push(data.heap); cpuData.push(data.cpu); if (labels.length > 20) { labels.shift(); heapData.shift(); cpuData.shift(); } performanceChart.update(); }).catch(error => console.error('Error fetching performance data:', error)); } setInterval(updatePerformance, 1000); updatePerformance();</script></body></html>");
    file.close();
}