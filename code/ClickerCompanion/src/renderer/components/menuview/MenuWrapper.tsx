import '../../globals.css';
import '../../fonts.css';
import 'tailwindcss/tailwind.css';
import React from 'react';
import { Props } from '../../types';

import PageWrapper from '../PageWrapper';
import Topbar from './Topbar';
import Sidebar from './Sidebar';

type IProps = {
  children: React.ReactNode;
};

export default function MenuWrapper({
  setCurrentPage,
  children,
}: Props & IProps) {
  return (
    <PageWrapper>
      <div className="flex min-h-screen max-h-screen">
        <Sidebar setCurrentPage={setCurrentPage} />
        <div className="flex flex-col flex-1 max-h-screen">
          <Topbar />
          <div className="flex h-screen max-h-screen overflow-y-hidden bg-[#000]/60 backdrop-saturate-[80%] t-2 mb-4 mt-2 ml-2 mr-4 rounded-default">{children}</div>
        </div>
      </div>
    </PageWrapper>
  );
}
