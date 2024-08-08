function uploadFile() {
    const fileInput = document.getElementById('fileInput');
    const formData = new FormData();
    formData.append('file', fileInput.files[0]);

    fetch('/upload', {
        method: 'POST',
        body: formData
    }).then(response => {
        if (response.ok) {
            alert('File uploaded successfully');
        } else {
            alert('File upload failed');
        }
    }).catch(error => {
        console.error('Error:', error);
    });
}

function readFile() {
    const filePath = document.getElementById('filePath').value;

    fetch(`/read?path=${encodeURIComponent(filePath)}`)
        .then(response => response.text())
        .then(data => {
            document.getElementById('fileContent').textContent = data;
        })
        .catch(error => {
            console.error('Error:', error);
        });
}

function deleteFile() {
    const filePath = document.getElementById('deleteFilePath').value;

    fetch(`/delete?path=${encodeURIComponent(filePath)}`)
        .then(response => {
            if (response.ok) {
                alert('File deleted successfully');
            } else {
                alert('File deletion failed');
            }
        })
        .catch(error => {
            console.error('Error:', error);
        });
}

function listFiles() {
    fetch('/listFiles')
        .then(response => response.json())
        .then(data => {
            const fileList = document.getElementById('fileList');
            fileList.innerHTML = '';
            data.files.forEach(file => {
                const li = document.createElement('li');
                li.textContent = file;
                fileList.appendChild(li);
            });
        })
        .catch(error => console.error('Error listing files:', error));
}