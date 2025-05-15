function loadSourceFiles(...files) {
    const modal = document.createElement('div');
    modal.style.position = 'fixed';
    modal.style.left = '0';
    modal.style.top = '0';
    modal.style.width = '100%';
    modal.style.height = '100%';
    modal.style.backgroundColor = 'rgba(0,0,0,0.8)';
    modal.style.zIndex = '1000';
    modal.style.overflowY = 'auto';

    const content = document.createElement('div');
    content.style.backgroundColor = '#fff';
    content.style.margin = '50px auto';
    content.style.padding = '20px';
    content.style.width = '90%';
    content.style.maxWidth = '800px';
    content.style.borderRadius = '8px';
    content.style.boxShadow = '0 4px 8px rgba(0, 0, 0, 0.2)';

    const closeBtn = document.createElement('button');
    closeBtn.textContent = 'Close';
    closeBtn.style.float = 'right';
    closeBtn.style.marginBottom = '20px';
    closeBtn.style.padding = '15px 30px';
    closeBtn.style.fontSize = '1.5rem';
    closeBtn.style.backgroundColor = '#007bff';
    closeBtn.style.color = '#fff';
    closeBtn.style.border = 'none';
    closeBtn.style.borderRadius = '8px';
    closeBtn.style.cursor = 'pointer';
    closeBtn.onclick = () => document.body.removeChild(modal);

    closeBtn.onmouseover = () => (closeBtn.style.backgroundColor = '#0056b3');
    closeBtn.onmouseout = () => (closeBtn.style.backgroundColor = '#007bff');

    content.appendChild(closeBtn);

    files.forEach(file => {
        fetch(file)
            .then(response => response.text())
            .then(text => {
                const fileContainer = document.createElement('div');
                fileContainer.style.marginTop = '20px';
                const fileName = file.split('/').pop();
                const fileNameHeader = document.createElement('h3');
                fileNameHeader.textContent = `File: ${fileName}`;
                fileNameHeader.style.marginBottom = '10px';
                fileNameHeader.style.fontSize = '18px';
                fileNameHeader.style.color = '#333';
                fileContainer.appendChild(fileNameHeader);

                const preContainer = document.createElement('div');
                preContainer.style.overflowX = 'auto';

                const pre = document.createElement('pre');
                pre.textContent = text;
                pre.style.whiteSpace = 'pre-wrap';
                pre.style.wordWrap = 'break-word';
                preContainer.appendChild(pre);

                fileContainer.appendChild(preContainer);
                content.appendChild(fileContainer);
            })
            // Most basic error handling I could think of...
            .catch(error => {
                console.error('Error loading file:', error);
                const errorMsg = document.createElement('p');
                errorMsg.textContent = `Error loading ${file}`;
                errorMsg.style.color = '#d9534f';
                content.appendChild(errorMsg);
            });
    });

    modal.appendChild(content);
    document.body.appendChild(modal);
}
