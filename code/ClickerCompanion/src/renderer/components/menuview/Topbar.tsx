import React from 'react';
import '../../globals.css';
import '../../fonts.css';
import 'tailwindcss/tailwind.css';

export default function Topbar() {
  return (
    <div className="flex h-[75px] bg-[#000]/60 backdrop-saturate-[80%] mt-4 mr-4 ml-2 mb-2 rounded-default items-center justify-between flex-shrink-0">
      <div className="p-3 mr-3 text-[18px]">
        <p>ClassClicker not connected</p>
      </div>
      <div className="flex flex-row">
        <div className="rounded-[8px] bg-[#f33] p-3 mr-3">
          <p>NOT CONNECTED</p>
        </div>
        <div className="rounded-[8px] bg-[#1af] p-3 mr-3 cursor-pointer transition-colors duration-250 ease-in-out hover:bg-[#45bdff] ">
          <p>CONNECT</p>
        </div>
      </div>
    </div>
  );
}
