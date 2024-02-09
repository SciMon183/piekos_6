document.addEventListener('DOMContentLoaded', function() {
    const imageContainer = document.getElementById('imageContainer');
    const images = ['klient/main/img/smok.jpg', 'klient/main/img/ladne.jpg', 'klient/main/img/ladne1.jpg'];
    let currentIndex = 0;
    let imageCache = [];

    // Cache obrazków
    images.forEach(function(imageUrl) {
        const img = new Image();
        img.src = imageUrl;
        imageCache.push(img);
    });

    // Zmiana obrazka
    function changeImage() {
        currentIndex = (currentIndex + 1) % images.length;
        imageContainer.innerHTML = '';
        imageContainer.appendChild(imageCache[currentIndex]);
    }

    // Zmiana obrazka co 5 sekund
    setInterval(changeImage, 5000);

    // Przyciski nawigacyjne (opcjonalne)
    const prevButton = document.getElementById('prevButton');
    const nextButton = document.getElementById('nextButton');

    prevButton.addEventListener('click', function() {
        currentIndex = (currentIndex - 1 + images.length) % images.length;
        changeImage();
    });

    nextButton.addEventListener('click', function() {
        currentIndex = (currentIndex + 1) % images.length;
        changeImage();
    });
});
