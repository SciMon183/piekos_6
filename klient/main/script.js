document.addEventListener('DOMContentLoaded', function()
{
    const imageContainer = document.getElementById('imageContainer');
    const images = ['/klient/main/img/ladne.jpg', '/klient/main/img/smok.jpg'];
    let currentIndex = 0;
    const imageElement = document.querySelector('#imageContainer img');

    function changeImage() 
    {
        currentIndex = (currentIndex + 1) % images.length;
        imageElement.style.opacity = '0';

        setTimeout(function() 
        {
            imageElement.src = images[currentIndex];
            imageElement.style.opacity = '1';
        }, 600); // Czas opóźnienia
    }

    // Uruchomienie funkcji zmiany obrazka co 6 sekund
    setInterval(changeImage, 6000);
});
