import React, { useState, useEffect } from 'react';

function App() {
  const [data, setData] = useState({ temperature: 0, humidity: 0 });

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
    const intervalId = setInterval(fetchData, 10000); // Refresh data every 30 seconds

    return () => clearInterval(intervalId);
  }, []);

  return (
    <div className="App">
      <header className="App-header">
        <h1>Temperature & Humidity Dashboard</h1>
        <h3>Current Status: {data.status}</h3>
        <p>Temperature: {data.temperature.toFixed(2)} °C</p>
        <p>Humidity: {data.humidity.toFixed(2)}%</p>
      </header>
    </div>
  );
}

export default App;
