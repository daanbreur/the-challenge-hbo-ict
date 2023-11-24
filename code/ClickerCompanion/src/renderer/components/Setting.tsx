import React from 'react';

type IProps = {
  children: React.ReactNode;
};

export default function Setting({ children }: IProps) {
  return (
    <div className="flex flex-nowrap justify-between mx-20">
      <div className="p-2 flex justify-center items-center">
        <p>SETTING</p>
      </div>
      <div className="p-2 flex justify-center items-center">
        <p>SETTINGSELECTOR</p>
      </div>
    </div>
  );
}
