/* eslint-disable jsx-a11y/no-static-element-interactions */
/* eslint-disable jsx-a11y/click-events-have-key-events */
import React from 'react';
import '../globals.css';
import '../fonts.css';
import 'tailwindcss/tailwind.css';
import PageWrapper from '../components/PageWrapper';
import { ISettings, Props } from '../types';
import Setting from '../components/Setting';
import SettingsGroup from '../components/SettingsGroup';

export default function Settings({
  setCurrentPage,
  // eslint-disable-next-line @typescript-eslint/no-unused-vars
  setSettings,
}: Props & {
  setSettings: React.Dispatch<React.SetStateAction<ISettings>>;
}) {
  return (
    <PageWrapper>
      <div className="flex justify-between mb-2">
        <div
          onClick={() => {
            setCurrentPage('overview');
          }}
          className="flex grow-0 shrink-0 px-8 text-xl cursor-pointer bg-cc_blue justify-center items-center rounded-lg my-2 ml-4 hover:outline font-bold"
        >
          Menu
        </div>
        <div className="text-center text-3xl">
          <span className="text-cc_blue">S</span>ettings
        </div>
        <div className="flex grow-0 shrink-0 px-8 text-xl cursor-pointer bg-button3 justify-center items-center rounded-lg my-2 mr-4 hover:outline font-bold">
          Reset
        </div>
      </div>

      <div className="h-full flex flex-col flex-nowrap mx-2 rounded-lg mb-5">
        <SettingsGroup name="Video">
          <Setting name="Test Setting">
            <p>toggle</p>
          </Setting>
          <Setting name="Test Setting 2">
            <p>dropdown</p>
          </Setting>
        </SettingsGroup>
        <SettingsGroup name="Audio">
          <Setting name="Andere Setting">
            <p>ook opties</p>
          </Setting>
        </SettingsGroup>
      </div>
      <hr className="h-[3px] bg-light_background my-2 w-90 m-auto border-0" />
      <footer className="text-center pt-5 m-auto ">
        ClickerCompanion by ClassClick B.V. 2023
      </footer>
    </PageWrapper>
  );
}
