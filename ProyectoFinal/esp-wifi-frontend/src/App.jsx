import { useState, useCallback } from 'react';
import io from 'socket.io-client';
import './App.css';

const socket = io('http://localhost:3000', {
  reconnection: true,
  reconnectionAttempts: 5,
  reconnectionDelay: 3000,
});

function App() {
  const [sliderValue, setSliderValue] = useState(5);

  const handleSliderChange = useCallback((e) => {
    const value = parseInt(e.target.value);
    setSliderValue(value);
    socket.emit('sliderValue', value);
  }, []);

  return (
    <>
      <h1>Communicate ESP-01 to stm32 UART</h1>
      <div className="slider-container">
        <input
          type="range"
          min="1"
          max="10"
          value={sliderValue}
          onChange={handleSliderChange}
          className="slider"
        />
        <span className="slider-value">{sliderValue}</span>
      </div>
    </>
  );
}

export default App;