import { useState } from 'react';
import { MemoryRouter as Router, Routes, Route } from 'react-router-dom';

import Quiz from './pages/Quiz';
// eslint-disable-next-line import/no-cycle
import Overview from './pages/Overview';
import Devices from './pages/Devices';
import Settings from './pages/Settings';

import { IPageNames, IQuiz, ISettings } from './types';

import './globals.css';
import './fonts.css';
import 'tailwindcss/tailwind.css';

function Renderer() {
  const [currentPage, setCurrentPage] = useState<IPageNames>('overview');
  const [selectedQuiz, setSelectedQuiz] = useState<IQuiz>({} as IQuiz);
  const [settings, setSettings] = useState<ISettings>({} as ISettings);
  // const [connectedDevices, setConnectDevices] = useState();

  if (currentPage === 'quiz-selection')
    return (
      <Quiz
        setCurrentPage={setCurrentPage}
        setSelectedQuiz={setSelectedQuiz}
        selectedQuiz={selectedQuiz}
        settings={settings}
      />
    );
  if (currentPage === 'devices')
    return (
      <Devices
        setCurrentPage={setCurrentPage}
        setSelectedQuiz={setSelectedQuiz}
        selectedQuiz={selectedQuiz}
        settings={settings}
      />
    );
  if (currentPage === 'settings')
    return (
      <Settings
        setCurrentPage={setCurrentPage}
        setSelectedQuiz={setSelectedQuiz}
        selectedQuiz={selectedQuiz}
        setSettings={setSettings}
        settings={settings}
      />
    );

  return (
    <Overview
      setCurrentPage={setCurrentPage}
      setSelectedQuiz={setSelectedQuiz}
      selectedQuiz={selectedQuiz}
      settings={settings}
    />
  );
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
