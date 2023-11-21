import { MemoryRouter as Router, Routes, Route } from 'react-router-dom';
import logo from '../../assets/logo.svg';
// import './App.css';
import 'tailwindcss/tailwind.css';

// async function requestPort() {
//   let port: SerialPort | null = null;
//   navigator.serial
//     .requestPort()
//     .then((_port) => (port = _port))
//     .catch((_a) => console.error(_a));

//   if (port == null) return;
//   const portInfo = port.getInfo();
//   console.log(port, portInfo);
// }

function Hello() {
  return (
    <div>
      <div className="Hello">
        <img width="200" alt="icon" src={logo} />
      </div>
      <div className="grid grid-cols-2 gap-4">
        <div className="">1</div>
        <div className="">2</div>
        <div className="">3</div>
        <div className="">4</div>
      </div>
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
