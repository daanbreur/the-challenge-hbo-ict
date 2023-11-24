/* eslint-disable jsx-a11y/no-static-element-interactions */
/* eslint-disable jsx-a11y/click-events-have-key-events */
import React from 'react';
import '../globals.css';
import '../fonts.css';
import 'tailwindcss/tailwind.css';
import PageWrapper from '../components/PageWrapper';
import { Props } from '../types';
import Setting from '../components/Setting';

export default function Settings({ setCurrentPage }: Props) {
  return (
    <PageWrapper>
      <div className="flex justify-between">
        <div
          onClick={() => {
            setCurrentPage('mainmenu');
          }}
          className="flex grow-0 shrink-0 px-8 text-xl cursor-pointer bg-cc_blue justify-center items-center rounded-lg m-2 hover:outline">
          menu
        </div>
        <div className="text-center text-3xl">Settings</div>
        <div className="flex grow-0 shrink-0 px-8 text-xl cursor-pointer bg-button3 justify-center items-center rounded-lg m-2 hover:outline">
          reset
        </div>
      </div>
      <div className="">
        <div className="h-full flex flex-col flex-nowrap mx-2 bg-light_background rounded-lg mb-5">
          <div className="p-2 flex justify-center items-center">
            <p>SubSettings 1</p>
          </div>
          <Setting />
          <div className="p-2 flex justify-center items-center">
            <p>SubSettings 2</p>
          </div>
          <Setting />
        </div>
      </div>
      <footer className="text-center pt-5 m-auto ">
        ClickerCompanion by ClassClick B.V. 2023
      </footer>
    </PageWrapper>
  );
}
