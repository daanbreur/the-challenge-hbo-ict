import React from 'react';

type IProps = {
  name: String;
  children: React.ReactNode;
};

export default function Setting({ children, name }: IProps) {
  return (
    <div className="grid grid-cols-2 justify-items-center mx-20">
      <div className="p-2">
        <p>{name}</p>
      </div>
      <div className="p-2">{children}</div>
    </div>
  );
}
