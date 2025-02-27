import { useEffect, useState } from "react";

const Status = () => {
  const [logs, setLogs] = useState([]);
  const [serverStatus, setServerStatus] = useState("Cargando...");

  useEffect(() => {
    const eventSource = new EventSource("http://localhost:5000/logs");

    eventSource.onmessage = (event) => {
      const newLog = JSON.parse(event.data);
      setLogs((prevLogs) => [...prevLogs, newLog]);

      setServerStatus("En funcionamiento");
    };

    eventSource.onerror = () => {
      setServerStatus("Desconectado");
      eventSource.close();
    };

    return () => eventSource.close();
  }, []);

  return (
    <section className="estado-servidor">
      <h2>Estado del Servidor</h2>
      <div className="estado">
        <span
          className="indicador"
          style={{ backgroundColor: serverStatus === "En funcionamiento" ? "green" : "red" }}
        ></span>
        <span>{serverStatus}</span>
      </div>
      <p>Pequeña descripción</p>
      <div className="logs">
        <h3>Logs:</h3>
        <ul>
          {logs.map((log, index) => (
            <li key={index}>{log.message}</li>
          ))}
        </ul>
      </div>
    </section>
  );
};

export default Status;
