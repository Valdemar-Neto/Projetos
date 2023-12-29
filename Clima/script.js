const inputBox = document.querySelector('.caixa-texto');
const searchBtn = document.getElementById('searchBtn');
const weather_img = document.querySelector('.weather-image');
const temperature = document.querySelector('.temperature');
const description = document.querySelector('.desc');
const humidity = document.getElementById('humidity');
const wind_spped = document.getElementById('wind-speed');

const location_not_found = document.querySelector('.location-not-found');

const weather_body = document.querySelector('.weather-body');
async function checkWeather(city){
    const api_key = "a0c59df354b939b5bccf5056aa1ed776";
    const url = `https://api.openweathermap.org/data/2.5/weather?q=${city}&appid=${api_key}`;

    const weather_data = await fetch(`${url}`).then(response => response.json());

    console.log(weather_data);

    if(weather_data.cod === '404'){
        location_not_found.style.display =  'flex';
        weather_body.style.display = 'none';
        console.log("error");
        return;
    }

    location_not_found.style.display =  'none';
    weather_body.style.display = 'flex';


    temperature.innerHTML = `${Math.round(weather_data.main.temp - 273.15)}°C`;
    description.innerHTML = `${weather_data.weather[0].description}`;
    humidity.innerHTML = `${weather_data.main.humidity}`;
    wind_spped.innerHTML = `${weather_data.wind.speed}`;

    /// Criando switch case para o tempo e suas respectivas imagens

    switch (weather_data.weather[0].main){
        case 'Clouds':
            weather_img.src = "/assets/cloudy.png";
            break;
        case 'Clear':
            weather_img.src = "/assets/clear.png";
            break;
        case 'Rain':
            weather_img.src = "/assets/rain.png";
            break;
        case 'Snow':
            weather_img.src = "/assets/snow.png";
            break;
    }
}

searchBtn.addEventListener('click', ()=>{
    checkWeather(inputBox.value);
});