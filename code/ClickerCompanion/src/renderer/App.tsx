import { MemoryRouter as Router, Routes, Route } from 'react-router-dom';
import logo from '../../assets/logo.svg';
import './globals.css';
import './fonts.css';
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
    <div className="dark min-h-screen text-text font-body bg-background">
      <div className="h-[40vh] flex justify-center items-center py-5">
        <img draggable={false} className="" width="20%" alt="icon" src={logo} />
      </div>
      <div className="h-[50vh] grid grid-cols-2 gap-4">
        <div className="flex-auto flex justify-center items-center hover:outline bg-button1 text-2xl rounded-lg ml-10 text-center">
          Start
        </div>
        <div className="flex-auto flex justify-center items-center hover:outline bg-button2 text-2xl rounded-lg mr-10 text-center">
          Quiz
        </div>
        <div className="flex-auto flex justify-center items-center hover:outline bg-button3 text-2xl rounded-lg ml-10 text-center">
          Devices
        </div>
        <div className="flex-auto flex justify-center items-center hover:outline bg-button4 text-2xl rounded-lg mr-10 text-center">
          Settings
        </div>
      </div>
      <footer className="text-center pt-5">
        ClickerCompanion by ClassClick B.V. 2023
      </footer>
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
