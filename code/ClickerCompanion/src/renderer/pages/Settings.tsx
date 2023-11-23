/* eslint-disable jsx-a11y/no-static-element-interactions */
/* eslint-disable jsx-a11y/click-events-have-key-events */
import React from 'react';
import '../globals.css';
import '../fonts.css';
import 'tailwindcss/tailwind.css';
import PageWrapper from '../components/PageWrapper';
import { Props } from '../types';

export default function Settings({ setCurrentPage }: Props) {
  return (
    <PageWrapper>
      <div className="flex">
        <div
          onClick={() => {
            setCurrentPage('mainmenu');
          }}
          className="flex-1 text-xl"
        >
          &lt; main menu
        </div>
        <div className="text-center text-3xl flex-2">Settings</div>
        <div className="flex-1"> </div>
      </div>
      <div className="h-auto">
        <div className="bg-lightbackground rounded-lg w-full h-auto mb-5">
          <p className="text-center mb-3">Quiz</p>
          <div>Setting 1</div>
        </div>
        <div className="bg-lightbackground rounded-lg w-full h-auto mb-5">
          <p className="text-center mb-3">Video</p>
          <div>
            <input type="range" min="1" max="100" value="50" />
          </div>
        </div>
        <div className="bg-lightbackground rounded-lg w-full h-auto">
          <p className="text-center mb-3">Audio</p>
          <div>Setting 1</div>
        </div>
      </div>
      <footer className="text-center pt-5">
        ClickerCompanion by ClassClick B.V. 2023
      </footer>
    </PageWrapper>
  );
}
