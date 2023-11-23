import '../globals.css';
import '../fonts.css';
import 'tailwindcss/tailwind.css';
import PageWrapper from '../components/PageWrapper';
import { Props } from '../types';

export default function Devices({ setCurrentPage }: Props) {
  return (
    <PageWrapper>
      <div className="text-center text-3xl">Devices</div>
      <div className="grid grid-cols-1">
        <div className="bg-lightbackground rounded-lg width-full">Quiz</div>
        <div className="bg-lightbackground rounded-lg">Display</div>
        <div className="bg-lightbackground rounded-lg">Audio</div>
      </div>
      <footer className="text-center pt-5">
        ClickerCompanion by ClassClick B.V. 2023
      </footer>
    </PageWrapper>
  );
}
