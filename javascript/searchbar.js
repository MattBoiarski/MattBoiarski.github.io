document.getElementById('searchInput').addEventListener('keydown', function (event) {
    if (event.key === 'Enter') {
        event.preventDefault();
        performSearch();
    }
});

document.getElementById('toggleSearch').addEventListener('click', function (event) {
    event.preventDefault();
    performSearch();
});

function performSearch() {
    clearHighlights();
    const searchInput = document.getElementById('searchInput').value.trim();
    const mainContent = document.querySelector('main');
    if (searchInput === '') return;
    // Escape special characters for use in a regex (REGEX SUPREMACY)
    const escapedInput = searchInput.replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
    const regex = new RegExp(escapedInput, 'gi');
    function highlightTextNodes(node) {
        if (node.nodeType === Node.TEXT_NODE) {
            const matches = [...node.nodeValue.matchAll(regex)];
            if (matches.length > 0) {
                const fragment = document.createDocumentFragment();
                let lastIndex = 0;
                matches.forEach(match => {
                    fragment.appendChild(document.createTextNode(node.nodeValue.slice(lastIndex, match.index)));
                    const span = document.createElement('mark');
                    span.className = 'highlight';
                    span.textContent = match[0];
                    fragment.appendChild(span);
                    lastIndex = match.index + match[0].length;
                });
                fragment.appendChild(document.createTextNode(node.nodeValue.slice(lastIndex)));

                node.parentNode.replaceChild(fragment, node);
            }
        } else if (node.nodeType === Node.ELEMENT_NODE) {
            node.childNodes.forEach(child => highlightTextNodes(child));
        }
    }
    highlightTextNodes(mainContent);
    const firstMatch = document.querySelector('.highlight');
    if (firstMatch) {
        firstMatch.scrollIntoView({ behavior: 'smooth', block: 'center' });
    }
}

function clearHighlights() {
    const highlights = document.querySelectorAll('.highlight');
    highlights.forEach(mark => {
        const parent = mark.parentNode;
        parent.replaceChild(document.createTextNode(mark.textContent), mark);
        parent.normalize(); // Combine adjacent text nodes
    });
}
