import React from 'react';

type IProps = {
  name: String;
  children: React.ReactNode;
};

export default function SettingsGroup({ children, name }: IProps) {
  return (
    <div>
      <div className="p-2 flex justify-center items-center text-xl">
        <hr className="h-[3px] bg-light_background my-2 w-full m-auto border-0" />
        <p className="absolute bg-background px-4">{name}</p>
      </div>
      {children}
    </div>
  );
}
