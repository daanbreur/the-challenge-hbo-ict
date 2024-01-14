import React from 'react';
import '../../globals.css';
import '../../fonts.css';
import 'tailwindcss/tailwind.css';
import StartButton from './StartButton';
import moment from 'moment/moment';

type IProps = {
  name: String;
  time: Date;
  players: Number;
};

export default function Report({ name, time, players }: IProps) {
  return (
    <div className="flex flex-row justify-between bg-[#000]/60 rounded-default h-[75px] min-h-[75px] max-h-[0%] w-full min-w-[400px] mb-1 overflow-hidden">
      <div className="text-left ml-2 ">
        <p className="">{name}</p>
        <p className="text-gray_text">
          {moment(time).format('D MMM YYYY HH:mm')} - {players.toString()}{' '}
          Players
        </p>
      </div>
      <div className="flex flex-row">
        <StartButton name="VIEW" />
      </div>
    </div>
  );
}
