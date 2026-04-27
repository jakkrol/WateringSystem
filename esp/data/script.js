let isFirstLoad = true;

async function fetchData() {
    try {
        const response = await fetch('/api/data');
        const data = await response.json();
        
        updateUI(data);

        if (isFirstLoad) {
            document.getElementById('dry-slider').value = data.drySet;
            document.getElementById('tank-input').value = data.tankSet;
            document.getElementById('dry-val-display').innerText = data.drySet;
            document.getElementById('tank-val-display').innerText = data.tankSet;
            isFirstLoad = false;
        }
    } catch (e) { console.error("Błąd danych"); }
}

function updateUI(data) {
    const dryThreshold = parseInt(document.getElementById('dry-slider').value);
    let sPer = Math.round(mapRange(data.soil, 1023, 200, 0, 100));
    sPer = Math.min(Math.max(sPer, 0), 100);
    
    document.getElementById('soil-val').innerText = sPer + '%';
    document.getElementById('soil-progress').setAttribute('stroke-dasharray', `${sPer}, 100`);
    
    const desc = document.getElementById('soil-desc');
    if(data.soil > dryThreshold) {
        desc.innerText = "SUCHO - PODLEWANIE"; desc.style.color = "#e74c3c";
    } else {
        desc.innerText = "WILGOTNOŚĆ OK"; desc.style.color = "#3498db";
    }

    document.getElementById('water-val').innerText = data.water + ' cm';
    document.getElementById('env-val').innerText = `${data.temp}°C / ${data.hum}%`;
    document.getElementById('press-val').innerText = data.press + ' hPa';
    document.getElementById('pump-card').className = data.pump ? "info-box active" : "info-box";
    document.getElementById('pump-val').innerText = data.pump ? "OTWARTY" : "ZAMKNIĘTY";
    document.getElementById('time').innerText = new Date().toLocaleTimeString();
}

function saveSettings() {
    const dry = document.getElementById('dry-slider').value;
    const tank = document.getElementById('tank-input').value;
    document.getElementById('dry-val-display').innerText = dry;
    document.getElementById('tank-val-display').innerText = tank;
    fetch(`/api/settings?dry=${dry}&tank=${tank}`);
}

function mapRange(v, iMin, iMax, oMin, oMax) { return (v-iMin)*(oMax-oMin)/(iMax-iMin)+oMin; }
setInterval(fetchData, 4000);
fetchData();