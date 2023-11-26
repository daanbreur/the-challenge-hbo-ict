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
      <div className="flex justify-center items-center py-5">
        <img
          draggable={false}
          className="h-[300px] min-h-[300px]"
          alt="icon"
          src={logo}
        />
      </div>
      <div className=" h-[50h] grid grid-cols-2 gap-6 font-bold text-3xl">
        <div
          onClick={() => {
            setCurrentPage('quiz-selection');
          }}
          className="flex justify-center items-center outline-text hover:outline bg-button1 rounded-lg text-center cursor-pointer py-10"
        >
          Start
        </div>
        <div
          onClick={() => {
            setCurrentPage('settings');
          }}
          className="flex justify-center items-center outline-text hover:outline bg-button2 rounded-lg text-center cursor-pointer py-10"
        >
          Settings
        </div>
        <div className="flex justify-center items-center outline-text hover:outline bg-button3 rounded-lg text-center cursor-pointer py-10">
          KK KNOP
        </div>
        <div className="flex justify-center items-center outline-text hover:outline bg-button4 rounded-lg text-center cursor-pointer py-10">
          NOG KNOP
        </div>
      </div>
      <footer className="text-center pt-5">
        ClickerCompanion by ClassClick B.V. 2023
      </footer>
    </PageWrapper>
  );
}
