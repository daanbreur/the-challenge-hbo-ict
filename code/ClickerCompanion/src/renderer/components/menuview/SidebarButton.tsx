import React from 'react';
import '../../globals.css';
import '../../fonts.css';
import 'tailwindcss/tailwind.css';
import { IPageNames } from '../../types';

type IProps = {
  name: String;
  children: React.ReactNode;
};

export default function SidebarButton({
  name,
  children,
  setCurrentPage,
}: IProps & {
  setCurrentPage: React.Dispatch<React.SetStateAction<IPageNames>>;
}) {
  return (
    // eslint-disable-next-line jsx-a11y/click-events-have-key-events
    <div
      onClick={() => {
        setCurrentPage(name);
      }}
      className="flex items-center ml-5 py-2 px-2 mb-2 text-left  cursor-pointer transition-colors duration-150 ease-in-out hover:text-accent"
    >
      <div className="">{children}</div>
      <p className="text-[32px] pl-4">{name}</p>
    </div>
  );
}
