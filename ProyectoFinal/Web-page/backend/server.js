const express = require('express');
const http = require('http');
const { Server } = require('socket.io');
const net = require('net');
const path = require('path');

const app = express();
const server = http.createServer(app);
let serverState = false;

// Configurar CORS para Socket.IO
const io = new Server(server, {
  cors: {
    origin: "http://localhost:5173", // URL de desarrollo de Vite
    methods: ["GET", "POST"]
  }
});

// Función para emitir logs
function broadcastLog(message) {
  console.log(message);
  io.emit('serverLog', message);
}

// Configuración del entorno
const PORT = process.env.PORT || 5000;

// Servir frontend en producción
if (process.env.NODE_ENV === 'production') {
  app.use(express.static(path.join(__dirname, '../frontend/dist')));
  app.get('*', (req, res) => {
    res.sendFile(path.resolve(__dirname, '../frontend/dist', 'index.html'));
  });
} else {
  app.get('/', (req, res) => {
    res.send('API funcionando en desarrollo');
  });
}

// Configuración Telnet
const espIp = '192.168.50.115';
const espPort = 23;
let telnetClient = null;

const connectToESP = () => {
  telnetClient = new net.Socket();

  telnetClient.connect(espPort, espIp, () => {
    broadcastLog(`[ESP] ${espIp}:${espPort} connected`);
  });

  telnetClient.on('data', (data) => {
    broadcastLog(`[ESP-RX] ${data.toString().trim()}`);
  });

  telnetClient.on('error', (err) => {
    broadcastLog(`[ESP] Error: ${err.message}`);
    setTimeout(connectToESP, 5000);
  });

  telnetClient.on('close', () => {
    broadcastLog('[ESP] Disconnected');
    setTimeout(connectToESP, 5000);
  });
};

connectToESP();

// Configurar Socket.IO
io.on('connection', (socket) => {
  serverState = true;
  broadcastLog(`[Web] Client connected: ${socket.id}`);

  socket.on('dataWifi', (value) => {
    if (!telnetClient || !telnetClient.writable) {
      broadcastLog('[ESP] Error connection');
      socket.emit('error', 'ESP not available');
      serverState = false;
      return;
    }

    try {
      const message = `${value}\n`;
      telnetClient.write(message);
      broadcastLog(`${message.trim()}`);
      socket.emit('acknowledge', { value, timestamp: Date.now() });
    } catch (err) {
      broadcastLog(`[ESP-TX] Error: ${err.message}`);
      socket.emit('error', 'Error sending data');
      serverState = false;
    }
  });


  socket.on('disconnect', () => {
    broadcastLog(`[Web] Client disconnected: ${socket.id}`);
    serverState = false;
  });
});

// Iniciar servidor
server.listen(PORT, () => {
  broadcastLog(`[Server] Hearing in http://localhost:${PORT}`);
});