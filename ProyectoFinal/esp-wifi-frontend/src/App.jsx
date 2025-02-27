import { useState, useCallback, useEffect } from 'react';
import io from 'socket.io-client';
import TemperatureNeedleChart from './assets/TemperatureCircularBar';
import HumedityNeedleChart from './assets/HumedityCircularBar';
import Status from './Status';
import './App.css';

const socket = io('http://localhost:3000', {
  reconnection: true,
  reconnectionAttempts: 5,
  reconnectionDelay: 3000,
});

let temp = 15;
let hum = 40;

function App() {
  const [sliderValue, setSliderValue] = useState(5);
  const [serverLogs, setServerLogs] = useState([]);  // Estado como array

  const handleSliderChange = useCallback((e) => {
    const value = parseInt(e.target.value);
    setSliderValue(value);
    socket.emit('sliderValue', value);
  }, []);

  useEffect(() => {
    socket.on('serverLog', (message) => {
      setServerLogs((prevLogs) => {
        // Agrega el nuevo mensaje y mantiene máximo 10 logs
        const updatedLogs = [...prevLogs, message];
        return updatedLogs.length > 10 
          ? [updatedLogs[0], ...updatedLogs.slice(-9)]  // Mantiene el mensaje inicial + últimos 9
          : updatedLogs;
      });
    });

    return () => {
      socket.off('serverLog');
    };
  }, []);

  return (
    <>
      <header>
        <h1>Domotización - Control de Bombilla</h1>
      </header>
      
      <div className='grid-container'>
        
        <section className="bombilla">
          <h2>Bombilla</h2>
          <input
            type="range"
            min="0"
            max="10"
            value={sliderValue}
            onChange={handleSliderChange}
            className="slider"
          />
          <span className="slider-value">{sliderValue}</span>
          <p>Description: lorem ipsum</p>
        </section>

        
        <section className="sensor">
            <h2>Sensor de Temperatura y Humedad</h2>
            <div className="sensor">
                <div className="datos-sensor">
                    <div className="dato">
                      <TemperatureNeedleChart temp={temp}/>
                      <p className="sensor-info">
                        <span className="sensor-titulo">Temperatura:</span>
                        <span className="sensor-valor">{temp}°C</span>
                      </p>
                    </div>
                    <div className="dato">
                      <HumedityNeedleChart hum={hum}/>
                      <p className="sensor-info">
                        <span className="sensor-titulo">Humedad:</span>
                        <span className="sensor-valor">{hum}%</span>
                      </p>
                    </div>
                </div>
            </div>
        </section>

        {/* Muestra el estado del servidor y logs en tiempo real */}
        <Status />

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