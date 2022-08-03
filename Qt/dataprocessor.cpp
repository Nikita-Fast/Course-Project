#include "dataprocessor.h"
#include "QtDebug"
#include "StrictRingBuffer.h"
#include <cmath>
#include "firfilter.h"


double imp_resp[] = {
    -0.001297449673640811,-0.0002766583475960106,-0.0003051365475834003,-0.000334783580098556,-0.00036564353778645797,-0.00039764775212865693,-0.0004308374552686771,-0.0004650739000630239,-0.0005004062650949322,-0.0005366288562195519,-0.0005737143269438651,-0.0006114755103293849,-0.0006497864675360278,-0.000688517674825909,-0.0007275737786558938,-0.0007667668280418203,-0.00080616938641145,-0.0008455475812299606,-0.0008848574909552676,-0.0009238024635857339,-0.0009622290689039907,-0.000999625880864409,-0.0010359779979025507,-0.0010708304772712714,-0.001104615027196392,-0.0011369962543423427,-0.0011683393697872447,-0.0011973287182430634,-0.0012236644853058298,-0.0012460629337352818,-0.0012695906026450243,-0.0012873388041345218,-0.0013023964166739464,-0.001314674802328091,-0.001322596772957099,-0.001327134670966246,-0.0013271636935573423,-0.0013231540713713019,-0.0013142719007204675,-0.0013007889120689982,-0.001281920826765557,-0.0012579530090180814,-0.0012281291472338047,-0.0011927655278357026,-0.0011512797070601902,-0.001103790898923458,-0.0010498205246133234,-0.0009894450183410225,-0.0009220951151456765,-0.0008478995279792593,-0.0007664990085996855,-0.0006780168462520045,-0.0005821739466606515,-0.00047886986902190905,-0.0003677931095074481,-0.0002489559645124226,-0.00012243966670253146,1.164246263391022e-05,0.00015361252114901654,0.0003040797869286675,0.000461741512956216,0.0006274740248694258,0.0008014175801794741,0.0009826328390883565,0.0011720082401606643,0.0013686412596361146,0.0015730791553101504,0.0017846670564545598,0.0020037604214812863,0.0022296834324255256,0.0024627798885406256,0.0027022478770158454,0.0029484477543175523,0.0032006393158643125,0.0034589440697550366,0.0037227852109266418,0.00399216288730532,0.004266326030751371,0.0045453291351262035,0.00482850994010843,0.005115838393855343,0.005406647265545278,0.005700718081187691,0.005997357715412633,0.006296402729438349,0.006597253830381489,0.006899693549712545,0.00720286391309217,0.0075065058870244705,0.007810306963437761,0.008113551269840764,0.008415541819467076,0.008716430264799785,0.00901489545551906,0.009311184338334253,0.009604149149735303,0.009893792850033443,0.010179126941047369,0.010460129609628928,0.010735771701288341,0.011006171010689387,0.011270187748830218,0.011527920750045292,0.011778446910241642,0.012021562149820777,0.012256567700991457,0.012483408830404889,0.012701226914472178,0.012909963265334133,0.01310888252795289,0.013297922542479326,0.013476444904347695,0.0136443836576757,0.013801103122995035,0.01394658000449768,0.014080230261150134,0.014202173679213276,0.014311763869306843,0.014409018689084365,0.0144936641539071,0.014565764619655289,0.014624653676856711,0.014670903108908897,0.014703714252444643,0.014723645263155578,0.014730135900300095,0.014723645263155578,0.014703714252444643,0.014670903108908897,0.014624653676856711,0.014565764619655289,0.0144936641539071,0.014409018689084365,0.014311763869306843,0.014202173679213276,0.014080230261150134,0.01394658000449768,0.013801103122995035,0.0136443836576757,0.013476444904347695,0.013297922542479326,0.01310888252795289,0.012909963265334133,0.012701226914472178,0.012483408830404889,0.012256567700991457,0.012021562149820777,0.011778446910241642,0.011527920750045292,0.011270187748830218,0.011006171010689387,0.010735771701288341,0.010460129609628928,0.010179126941047369,0.009893792850033443,0.009604149149735303,0.009311184338334253,0.00901489545551906,0.008716430264799785,0.008415541819467076,0.008113551269840764,0.007810306963437761,0.0075065058870244705,0.00720286391309217,0.006899693549712545,0.006597253830381489,0.006296402729438349,0.005997357715412633,0.005700718081187691,0.005406647265545278,0.005115838393855343,0.00482850994010843,0.0045453291351262035,0.004266326030751371,0.00399216288730532,0.0037227852109266418,0.0034589440697550366,0.0032006393158643125,0.0029484477543175523,0.0027022478770158454,0.0024627798885406256,0.0022296834324255256,0.0020037604214812863,0.0017846670564545598,0.0015730791553101504,0.0013686412596361146,0.0011720082401606643,0.0009826328390883565,0.0008014175801794741,0.0006274740248694258,0.000461741512956216,0.0003040797869286675,0.00015361252114901654,1.164246263391022e-05,-0.00012243966670253146,-0.0002489559645124226,-0.0003677931095074481,-0.00047886986902190905,-0.0005821739466606515,-0.0006780168462520045,-0.0007664990085996855,-0.0008478995279792593,-0.0009220951151456765,-0.0009894450183410225,-0.0010498205246133234,-0.001103790898923458,-0.0011512797070601902,-0.0011927655278357026,-0.0012281291472338047,-0.0012579530090180814,-0.001281920826765557,-0.0013007889120689982,-0.0013142719007204675,-0.0013231540713713019,-0.0013271636935573423,-0.001327134670966246,-0.001322596772957099,-0.001314674802328091,-0.0013023964166739464,-0.0012873388041345218,-0.0012695906026450243,-0.0012460629337352818,-0.0012236644853058298,-0.0011973287182430634,-0.0011683393697872447,-0.0011369962543423427,-0.001104615027196392,-0.0010708304772712714,-0.0010359779979025507,-0.000999625880864409,-0.0009622290689039907,-0.0009238024635857339,-0.0008848574909552676,-0.0008455475812299606,-0.00080616938641145,-0.0007667668280418203,-0.0007275737786558938,-0.000688517674825909,-0.0006497864675360278,-0.0006114755103293849,-0.0005737143269438651,-0.0005366288562195519,-0.0005004062650949322,-0.0004650739000630239,-0.0004308374552686771,-0.00039764775212865693,-0.00036564353778645797,-0.000334783580098556,-0.0003051365475834003,-0.0002766583475960106,-0.001297449673640811
};
FIRFilter low_pass(imp_resp, 251);

DataProcessor::DataProcessor(int osc_freq, QObject* parent) : QObject(parent), oscill_freq (osc_freq) {

}

void DataProcessor::setBuffer(StrictRingBuffer* buffer){
  this->buffer = buffer;
}

void DataProcessor::writePacketToBuf(short* packet, int length) {
  if (!is_paused) {

      short output[length];
      low_pass.filter(packet, length, output);
      for (int i = 0; i < length; i++) {
          buffer->write_or_rewrite(output[i]);
      }

//      for (int i = 0; i < length; i++) {
//        buffer->write_or_rewrite(packet[i]);
//      }
  }
}

void DataProcessor::set_is_paused_true() {
  is_paused = true;
}

void DataProcessor::set_is_paused_false() {
  is_paused = false;
}
