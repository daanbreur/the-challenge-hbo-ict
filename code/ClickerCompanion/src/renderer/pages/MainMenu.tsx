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
        <img draggable={false} className="h-[300px] min-h-[300px]" alt="icon" src={logo} />
      </div>
      <div className="grid grid-cols-2 gap-10 font-bold text-3xl">
        <div
          onClick={() => {
            setCurrentPage('quiz-selection');
          }}
          className=" flex justify-center items-center outline-text hover:outline bg-cc_blue rounded-lg text-center cursor-pointer"
        >
          Start
        </div>
        <div
          onClick={() => {
            setCurrentPage('settings');
          }}
          className="flex justify-center items-center outline-text hover:outline bg-button3 rounded-lg text-center cursor-pointer"
        >
          Settings
        </div>
        <div className="flex-auto flex justify-center items-center bg-button3 rounded-lg ml-10" />
        <div className="flex-auto flex justify-center items-center bg-button4 rounded-lg mr-10" />
      </div>
      <footer className="text-center pt-5">
        ClickerCompanion by ClassClick B.V. 2023
      </footer>
    </PageWrapper>
  );
}
