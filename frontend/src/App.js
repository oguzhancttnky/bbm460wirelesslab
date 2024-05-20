import React, { useState, useEffect } from 'react';

function App() {
  const [data, setData] = useState({ status: '', temperature: 0, humidity: 0 });

  useEffect(() => {
    const fetchData = async () => {
      try {
        const response = await fetch('http://localhost:8080/data');
        const json = await response.json();
        setData(json);
      } catch (error) {
        console.error('Error fetching data: ', error);
      }
    };

    fetchData();
    const intervalId = setInterval(fetchData, 10000); // Refresh data every 10 seconds

    return () => clearInterval(intervalId);
  }, []);

  const getStatusColor = (status) => {
    switch (status) {
      case 'High Risk!':
        return '#FF4433';
      case 'Moderate Risk':
        return '#FFAA33';
      case 'Low Risk':
        return '#FCF55F';
      case 'Safe':
        return '#228B22';
      default:
        return '#000';
    }
  };

  return (
    <div className="app">
      <header className="app-header">
        <h1 className="title">Wireless Over Temperature Alarm Project Dashboard</h1>
      </header>
      <main className="app-content">
        <div className="card">
          <h3 className="status">Fire Rate Status: <span style={{ color: getStatusColor(data.status) }}>{data.status}</span></h3>
          <span class="thermometer">{data.temperature.toFixed(2)}&deg;C</span>
          <p className="data">Temperature: <span style={{ color: getStatusColor(data.status) }}>{data.temperature.toFixed(2)} &deg;C</span></p>
          <p className="data">Humidity: <span style={{ color: getStatusColor(data.status) }}>{data.humidity.toFixed(2)}%</span></p>
        </div>
      </main>
    </div>
  );
}

export default App;
