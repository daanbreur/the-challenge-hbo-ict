/* eslint-disable jsx-a11y/no-static-element-interactions */
/* eslint-disable jsx-a11y/click-events-have-key-events */
import React from 'react';
import PageWrapper from '../components/PageWrapper';
import logo from '../../../assets/logo.svg';

import { Props } from '../types';

import '../globals.css';
import '../fonts.css';
import 'tailwindcss/tailwind.css';

export default function MainMenu({ setCurrentPage }: Props) {
  return (
    <PageWrapper>
      <div className="h-[40vh] flex justify-center items-center py-5">
        <img draggable={false} className="h-full" alt="icon" src={logo} />
      </div>
      <div className="h-[50vh] grid grid-cols-2 gap-4 text-black_text font-bold text-3xl">
        <div
          onClick={() => {
            setCurrentPage('quiz-selection');
          }}
          className="flex-auto flex justify-center items-center outline-text hover:outline bg-button1 rounded-lg ml-10 text-center cursor-pointer"
        >
          Start
        </div>
        <div
          onClick={() => {
            setCurrentPage('settings');
          }}
          className="flex-auto flex justify-center items-center outline-text hover:outline bg-button2 rounded-lg mr-10 text-center cursor-pointer"
        >
          Settings
        </div>
        <div className="flex-auto flex justify-center items-center outline-text hover:outline bg-button3 rounded-lg ml-10 text-center cursor-pointer" />
        <div className="flex-auto flex justify-center items-center outline-text hover:outline bg-button4 rounded-lg mr-10 text-center cursor-pointer" />
      </div>
      <footer className="text-center pt-5">
        ClickerCompanion by ClassClick B.V. 2023
      </footer>
    </PageWrapper>
  );
}
