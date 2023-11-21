import { MemoryRouter as Router, Routes, Route } from 'react-router-dom';
import logo from '../../assets/logo.svg';
// import './App.css';
import 'tailwindcss/tailwind.css';

async function requestPort() {
  let port: SerialPort | null = null;
  navigator.serial
    .requestPort()
    .then((_port) => (port = _port))
    .catch((_a) => console.error(_a));

  if (port == null) return;
  const portInfo = port.getInfo();
  console.log(port, portInfo);
}

function Hello() {
  return (
    <div>
      <div className="Hello">
        <img width="200" alt="icon" src={logo} />
      </div>
      <h1 className="text-3xl font-bold underline">
        electron-react-boilerplate
      </h1>
      <button type="button" onClick={() => requestPort()}>
        Pair a new port
      </button>
      {/* <button onClick={() => serial.openPort()}>Open the selected port</button>
      <button onClick={() => serial.startReading()}>Start reading</button> */}
    </div>
  );
}

export default function App() {
  return (
    <Router>
      <Routes>
        <Route path="/" element={<Hello />} />
      </Routes>
    </Router>
  );
}
