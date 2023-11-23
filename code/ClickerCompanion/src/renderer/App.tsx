import { useState } from 'react';
import { MemoryRouter as Router, Routes, Route } from 'react-router-dom';

import Quiz from './pages/Quiz';
import MainMenu from './pages/MainMenu';
import Devices from './pages/Devices';
import Settings from './pages/Settings';

import { IPageNames } from './types';

import './globals.css';
import './fonts.css';
import 'tailwindcss/tailwind.css';

function Renderer() {
  const [currentPage, setCurrentPage] = useState<IPageNames>('mainmenu');
  // const [selectedQuiz, setSelectedQuiz] = useState();
  // const [connectedDevices, setConnectDevices] = useState();

  if (currentPage === 'quiz-selection')
    return <Quiz setCurrentPage={setCurrentPage} />;
  if (currentPage === 'devices')
    return <Devices setCurrentPage={setCurrentPage} />;
  if (currentPage === 'settings')
    return <Settings setCurrentPage={setCurrentPage} />;

  return <MainMenu setCurrentPage={setCurrentPage} />;
}

export default function App() {
  return (
    <Router>
      <Routes>
        <Route path="/" element={<Renderer />} />
      </Routes>
    </Router>
  );
}
