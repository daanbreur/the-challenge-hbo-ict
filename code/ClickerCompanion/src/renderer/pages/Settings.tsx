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
      <div className="flex justify-between">
        <div
          onClick={() => {setCurrentPage('mainmenu');}}
          className="flex grow-0 shrink-0 px-8 text-xl cursor-pointer bg-cc_blue justify-center items-center rounded-lg m-2 hover:outline">
          menu
        </div>
        <div className="text-center text-3xl">Settings</div>
        <div className="flex grow-0 shrink-0 px-8 text-xl cursor-pointer bg-button3 justify-center items-center rounded-lg m-2 hover:outline">
          reset
        </div>

      </div>
      <div className="h-auto mx-2">
        <div className="bg-light_background rounded-lg w-full mb-5 ">
          <div className="p-2">
          Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin vitae venenatis risus, id lacinia mauris. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Morbi scelerisque risus tortor, id sagittis eros consequat sed. Duis ac eros sit amet leo ornare consectetur a sit amet justo. Sed egestas enim vel arcu consequat laoreet. Sed venenatis massa eu accumsan volutpat. Donec vel auctor orci, eu ullamcorper diam. Proin quis nulla volutpat, fermentum ligula vel, finibus ante. Etiam ex elit, viverra id cursus nec, vestibulum id urna.
          </div>
        </div>
      </div>
      <footer className="text-center pt-5 m-auto ">
        ClickerCompanion by ClassClick B.V. 2023
      </footer>
    </PageWrapper>
  );
}
