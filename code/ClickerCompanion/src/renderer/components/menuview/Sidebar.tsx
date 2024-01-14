import React from 'react';
import '../../globals.css';
import '../../fonts.css';
import 'tailwindcss/tailwind.css';
import * as Icons from 'react-icons/bi';

import SidebarButton from './SidebarButton';
import { IPageNames } from '../../types';
import logo from '../../../../assets/logo.svg';

export default function Sidebar({
  setCurrentPage,
}: {
  setCurrentPage: React.Dispatch<React.SetStateAction<IPageNames>>;
}) {
  return (
    <div className="flex flex-col">
      <div className="flex bg-[#000]/60 backdrop-saturate-[80%] w-[250px] rounded-default h-[120px] items-center justify-center ml-4 mt-4 p-2">
        <img draggable={false} alt="icon" src={logo} />
      </div>
      <div className="flex flex-col flex-grow w-[250px] bg-[#000]/60 backdrop-saturate-[80%] my-4 ml-4 mr-2 rounded-default shrink-0">
        <div className="flex flex-col justify-center">
          <SidebarButton name="home" setCurrentPage={setCurrentPage}>
            <Icons.BiHomeAlt2 size={40} />
          </SidebarButton>
          <SidebarButton name="quizzes" setCurrentPage={setCurrentPage}>
            <Icons.BiChalkboard size={40} />
          </SidebarButton>
          <SidebarButton name="reports" setCurrentPage={setCurrentPage}>
            <Icons.BiLineChart size={40} />
          </SidebarButton>
          <SidebarButton name="settings" setCurrentPage={setCurrentPage}>
            <Icons.BiCog size={40} />
          </SidebarButton>
        </div>
      </div>
      <div className="flex text-[12px] w-[250px] rounded-default flex-col text-gray_text ml-4 mb-4 py-2 bg-[#000]/60 backdrop-saturate-[80%] text-center mt-auto justify-between">
        <p>Copyright © 2024 ClassClick B.V.</p>
        <p>FOR</p>
        <p>De Haagse Hogeschool</p>
        <p>BY</p>
        <p>Thijs Kamphuis</p>
        <p>Daan Breur</p>
        <p>Manu Kapel</p>
        <p>Martijn Swarte</p>
        <p>Jeroen de Kuijper</p>
      </div>
      <div className="flex flex-row">
        <a
          className="mt-auto flex-grow rounded-default py-2 bg-[#000]/60 backdrop-saturate-[80%] text-center ml-4 mb-4 cursor-pointer transition-colors duration-150 ease-in-out hover:text-accent"
          href="https://classclick.nl"
          target="_blank"
          rel="noreferrer"
        >
          classclick.nl
        </a>
        <a
          className="mt-auto w-[50px] rounded-default py-2 bg-[#000]/60 backdrop-saturate-[80%] flex justify-center ml-2 mb-4 mr-2 cursor-pointer transition-colors duration-150 ease-in-out hover:text-accent"
          href="https://classclicker.nl"
          target="_blank"
          rel="noreferrer"
          title="SOURCE CODE"
        >
          <Icons.BiCodeBlock size={24} />
        </a>
      </div>
    </div>
  );
}
