const express = require('express');
const http = require('http');
const { Server } = require('socket.io');
const net = require('net');

const app = express();
const server = http.createServer(app);
let serverState = false

// Configurar CORS para Socket.IO
const io = new Server(server, {
  cors: {
    origin: "http://localhost:5173", // URL de desarrollo de Vite
    methods: ["GET", "POST"]
  }
});

// Función para emitir logs a los clientes y mostrarlos en consola
function broadcastLog(message) {
  console.log(message);
  io.emit('serverLog', message);
}

// Configuración Telnet
const espIp = '192.168.218.115';
const espPort = 23;
let telnetClient = null;

// Función de conexión con reconexión automática
const connectToESP = () => {
  telnetClient = new net.Socket();

  telnetClient.connect(espPort, espIp, () => {
    broadcastLog(`[ESP-01] Conectado a ${espIp}:${espPort}`);
  });

  telnetClient.on('data', (data) => {
    broadcastLog(`[ESP-01] Dato recibido: ${data.toString().trim()}`);
  });

  telnetClient.on('error', (err) => {
    broadcastLog(`[ESP-01] Error: ${err.message}`);
    setTimeout(connectToESP, 5000);
  });

  telnetClient.on('close', () => {
    broadcastLog('[ESP-01] Conexión cerrada');
    setTimeout(connectToESP, 5000);
  });
};

// Iniciar conexión
connectToESP();

// Configurar Socket.IO
io.on('connection', (socket) => {
  serverState = true;
  broadcastLog(`[Web] Cliente conectado: ${socket.id}`);

  socket.on('sliderValue', (value) => {
    if (!telnetClient || !telnetClient.writable) {
      broadcastLog('[ESP-01] Conexión no disponible');
      socket.emit('error', 'Conexión con ESP-01 no disponible');
      serverState = false;
      return;
    }

    try {
      const message = `${value}\n`;
      telnetClient.write(message);
      broadcastLog(`[ESP-01] Enviado: ${message.trim()}`);
      socket.emit('acknowledge', { value, timestamp: Date.now() });
    } catch (err) {
      broadcastLog(`[Error] Envío fallido: ${err.message}`);
      socket.emit('error', 'Error al enviar dato al ESP-01');
      serverState = false;
    }
  });

  socket.on('disconnect', () => {
    broadcastLog(`[Web] Cliente desconectado: ${socket.id}`);
    serverState = false;
  });
});

// Iniciar servidor
const PORT = 3000;
server.listen(PORT, () => {
  broadcastLog(`[Servidor] Escuchando en http://localhost:${PORT}`);
});
