import { MemoryRouter as Router, Routes, Route } from 'react-router-dom';
import icon from '../../assets/icon.svg';
import './App.css';
import 'tailwindcss/tailwind.css';
import { useWebSerial } from '@mniota/react-webserial-hook';

function Hello() {
  const serial = useWebSerial({
    onData: (data) => {
      const decoder = new TextDecoder();
      console.log(decoder.decode(data));
    },
  });

  return (
    <div>
      <div className="Hello">
        <img width="200" alt="icon" src={icon} />
      </div>
      <h1>electron-react-boilerplate</h1>
      <button onClick={() => serial.requestPort()}>Pair a new port</button>
      <button onClick={() => serial.openPort()}>Open the selected port</button>
      <button onClick={() => serial.startReading()}>Start reading</button>
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
