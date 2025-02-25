import { defineConfig } from 'vite'
import react from '@vitejs/plugin-react'

// https://vite.dev/config/
export default defineConfig({
  plugins: [react()],
  server: {
    host: '0.0.0.0', // Permite acceso desde cualquier dispositivo en la red
    port: 5173, // Puedes cambiarlo si es necesario
    strictPort: true, // Fija el puerto para evitar cambios automáticos
    proxy: {
      // Redirigir peticiones a socket.io al puente Node.js
      '/socket.io': {
        target: 'http://localhost:3000',
        changeOrigin: true,
        ws: true,
      },
    },
  },
});
