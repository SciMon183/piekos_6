document.addEventListener('DOMContentLoaded', function() {
    const imageContainer = document.getElementById('imageContainer');
    const images = ['/klient/main/img/ladne.jpg', '/klient/main/img/smok.jpg'];
    let currentIndex = 0;

    function changeImage() {
        currentIndex = (currentIndex + 1) % images.length;
        const newImage = new Image();
        newImage.src = images[currentIndex];
        newImage.alt = 'Zdjęcie';
        newImage.classList.add('fade-in');

        newImage.onload = function() {
            imageContainer.innerHTML = '';
            imageContainer.appendChild(newImage);
        };
    }

    setInterval(changeImage, 6000);
});

// Funkcja do odświeżania strony
function odswiezStrone() {
    location.reload();
}

// Uruchomienie funkcji odświeżania co minutę (60 000 milisekund)
setInterval(odswiezStrone, 60000);
