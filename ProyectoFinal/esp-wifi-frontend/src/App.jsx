import { useState, useCallback } from 'react';
import io from 'socket.io-client';
import TemperatureNeedleChart from './assets/TemperatureCircularBar';
import HumedityNeedleChart from './assets/HumedityCircularBar';

import './App.css';

const socket = io('http://localhost:3000', {
  reconnection: true,
  reconnectionAttempts: 5,
  reconnectionDelay: 3000,
});

let temp = 15;
let hum = 20;

function App() {
  const [sliderValue, setSliderValue] = useState(5);

  const handleSliderChange = useCallback((e) => {
    const value = parseInt(e.target.value);
    setSliderValue(value);
    socket.emit('sliderValue', value);
  }, []);

  return (
    <>
      <header>
        <h1>Domotización - Control de Bombilla</h1>
      </header>
      
      <p>{temp}</p>
      <div className='grid-container'>
        
        <section className="bombilla">
          <h2>Bombilla</h2>
          <input
            type="range"
            min="1"
            max="10"
            value={sliderValue}
            onChange={handleSliderChange}
            className="slider"
          />
          <span className="slider-value">{sliderValue}</span>
          <p>Description: lorem ipsum</p>
        </section>

        //Sección del Sensor
        <section className="sensor">
            <h2>Sensor de Temperatura y Humedad</h2>
            <div className="sensor">
                <div className="datos-sensor">
                    <div className="dato">
                    <TemperatureNeedleChart temp={temp}/>
                    </div>
                    <div className="dato">
                      <HumedityNeedleChart hum={hum}/>
                    </div>
                </div>
                <div className="imagen-referencia">

                </div>
                <p>Pequeña descripción</p>
            </div>
        </section>
        //Sección del Estado del Servidor
        <section className="estado-servidor">
            <h2>Estado del Servidor</h2>
            <div className="estado-servidor">
                <div className="estado">
                    <span className="indicador" id="indicador-servidor"></span>
                    <span id="texto-estado">Cargando...</span>
                </div>
                <p>Pequeña descripción</p>
            </div>
            
        </section>

        //Sección de la Gráfica de Lumens
        <section className="grafica-lumens">
            <h2>Gráfica de Lumens</h2>
            <div className="grafica-tiempo-real">
                <h2>Gráfica de Lumens en Tiempo Real</h2>
                <div className="contenedor-grafica">
                    <canvas id="grafica-lumens"></canvas>
                </div>
                <p>Pequeña descripción</p>
            </div>
        </section>    
      </div>
    </>
  );
}

export default App;