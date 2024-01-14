import React from 'react';
import '../../globals.css';
import '../../fonts.css';
import 'tailwindcss/tailwind.css';

type IProps = {
  name: String;
};

export default function StartButton({ name }: IProps) {
  return (
    <div className={`flex items-center rounded-[5px] justify-center bg-accent w-[120px] max-w-[120px] h-[60px] m-2 cursor-pointer transition-colors duration-150 ease-in-out hover:bg-[#ff52b6] flex-shrink-0 ml-3 `}>
      <p>{name}</p>
    </div>
  );
}
