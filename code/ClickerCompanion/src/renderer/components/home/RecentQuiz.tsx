import React from 'react';
import '../../globals.css';
import '../../fonts.css';
import 'tailwindcss/tailwind.css';
import StartButton from './StartButton';

type IProps = {
  name: String;
  desc: String;
};

export default function RecentQuiz({ name, desc }: IProps) {
  return (
    <div className="flex flex-row justify-between bg-[#000]/60 rounded-default h-[75px] min-h-[75px] max-h-[0%] w-full min-w-[400px] mb-1 overflow-hidden">
      <div className="text-left ml-2">
        <p className="">{name}</p>
        <p className="text-gray_text">{desc}</p>
      </div>
      <div>
        <StartButton name="PLAY" />
      </div>
    </div>
  );
}
