import React from 'react';
import '../../globals.css';
import '../../fonts.css';
import 'tailwindcss/tailwind.css';

type IProps = {
  name: String;
  children: React.ReactNode;
};

export default function RecentList({ name, children }: IProps) {
  return (
    <div className="flex flex-col w-[50%] pt-2">
      <p className="flex justify-center text-[24px]">{name}</p>
      <div className="overflow-hidden flex-grow">
        <div className="flex flex-col max-h-screen ">
          <div className="flex flex-col flex-wrap gap-[19px] p-4">
            {children}
          </div>
        </div>
      </div>
    </div>
  );
}
