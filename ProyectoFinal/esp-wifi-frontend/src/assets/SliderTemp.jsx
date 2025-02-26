import { useState, useCallback } from 'react';
import io from 'socket.io-client';
import './SliderTemp.css';



function SliderTemp(hola) {
  //logic

  return (
    <>
      <input
      type="range"
      min="1"
      max="10"
      value={sliderValue}
      onChange={handleSliderChange}
      className="slider"
      />
      <h1>{hola}</h1>
    </>
  );
}

export default SliderTemp;