document.addEventListener('DOMContentLoaded', function() {
    const imageContainer = document.getElementById('imageContainer');
    const images = ['/klient/main/img/smok.jpg', '/klient/main/img/ladne.jpg'];  // Dodaj tutaj ścieżki do swoich obrazków
    let currentIndex = 0;

    function changeImage() {
        currentIndex = (currentIndex + 1) % images.length;
        const imageUrl = images[currentIndex];
        const newImage = new Image();
        newImage.src = imageUrl;

        // Dodajmy event listener, aby zmiana obrazka była płynna
        newImage.addEventListener('load', function() {
            const currentImage = imageContainer.querySelector('img');

            newImage.style.opacity = 0;  // Ustawienie przezroczystości nowego obrazka na 0
            imageContainer.appendChild(newImage);

            setTimeout(function() {
                currentImage.style.opacity = 0;  // Zanikanie aktualnego obrazka
                newImage.style.opacity = 1;  // Pojawianie się nowego obrazka
            }, 10);  // Krótka przerwa przed rozpoczęciem animacji, aby zapewnić płynność
        });
    }

    setInterval(changeImage, 8000);  // Zmiana obrazka co 30 sekund
});