import React from 'react';

type IProps = {
  children: React.ReactNode;
};

export default function PageWrapper({ children }: IProps) {
  return (
    <div className="select-none dark min-h-screen text-text font-body bg-background">
      {children}
    </div>
  );
}
