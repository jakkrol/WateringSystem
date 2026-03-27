
const THRESHOLD_SOIL_DRY = 700;    
const THRESHOLD_SOIL_WET = 500;  
const MAX_WATER_DIST = 50;         

async function fetchData() {
    try {
        const response = await fetch('/api/data');
        if (!response.ok) throw new Error('Błąd serwera');
        const data = await response.json();
        
        updateUI(data);
        document.getElementById('connection-status').classList.add('online');
    } catch (error) {
        console.error('Brak danych z ESP:', error);
        document.getElementById('connection-status').classList.remove('online');
    }
}

function updateUI(data) {

    let soilPercent = Math.round(mapRange(data.soil, 1023, 200, 0, 100));
    soilPercent = Math.min(Math.max(soilPercent, 0), 100); // ogranicz do 0-100
    
    document.getElementById('soil-val').innerText = soilPercent + '%';
    document.getElementById('soil-progress').setAttribute('stroke-dasharray', `${soilPercent}, 100`);
    
    
    const desc = document.getElementById('soil-desc');
    if(data.soil > THRESHOLD_SOIL_DRY) {
        desc.innerText = "SUCHO - WYMAGA PODLANIA";
        desc.style.color = "#e74c3c";
    } else if (data.soil < THRESHOLD_SOIL_WET) {
        desc.innerText = "BARDZO MOKRO";
        desc.style.color = "#2ecc71";
    } else {
        desc.innerText = "WILGOTNOŚĆ PRAWIDŁOWA";
        desc.style.color = "#3498db";
    }

  
    document.getElementById('water-val').innerText = data.water + ' cm';
    document.getElementById('env-val').innerText = `${data.temp}°C / ${data.hum}%`;
    document.getElementById('press-val').innerText = data.press + ' hPa';
    
    
    const pumpCard = document.getElementById('pump-card');
    const pumpVal = document.getElementById('pump-val');
    if(data.pump) {
        pumpCard.classList.add('active');
        pumpVal.innerText = "OTWARTY";
    } else {
        pumpCard.classList.remove('active');
        pumpVal.innerText = "ZAMKNIĘTY";
    }

    
    document.getElementById('time').innerText = new Date().toLocaleTimeString();
}


function mapRange(value, in_min, in_max, out_min, out_max) {
    return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

setInterval(fetchData, 5000);
fetchData(); 