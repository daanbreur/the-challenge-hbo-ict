import React from 'react';

type IProps = {
  children: React.ReactNode;
};

export default function PageWrapper({ children }: IProps) {
  return (
    <div className="dark text-text font-body bg-gradient-to-bl from-[#3e41b8] via-[#a855f7] to-accent select-none">
      {children}
    </div>
  );
}
