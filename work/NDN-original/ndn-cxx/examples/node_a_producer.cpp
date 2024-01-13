#include "base_producer.hpp"

// 継承クラスの定義
namespace ndn
{
  namespace examples
  {

    class MyProducer : public BaseProducer
    {
    public:
      void run() override
      {
        m_face.setInterestFilter("/A",
                                 std::bind(&BaseProducer::onInterest, this, _2),
                                 nullptr, // RegisterPrefixSuccessCallback is optional
                                 std::bind(&BaseProducer::onRegisterFailed, this, _1, _2));
        m_face.setInterestFilter("/B",
                                 std::bind(&BaseProducer::onInterest, this, _2),
                                 nullptr, // RegisterPrefixSuccessCallback is optional
                                 std::bind(&BaseProducer::onRegisterFailed, this, _1, _2));
        m_face.setInterestFilter("/C",
                                 std::bind(&BaseProducer::onInterest, this, _2),
                                 nullptr, // RegisterPrefixSuccessCallback is optional
                                 std::bind(&BaseProducer::onRegisterFailed, this, _1, _2));
        m_face.setInterestFilter("/D",
                                 std::bind(&BaseProducer::onInterest, this, _2),
                                 nullptr, // RegisterPrefixSuccessCallback is optional
                                 std::bind(&BaseProducer::onRegisterFailed, this, _1, _2));
        m_face.setInterestFilter("/E",
                                 std::bind(&BaseProducer::onInterest, this, _2),
                                 nullptr, // RegisterPrefixSuccessCallback is optional
                                 std::bind(&BaseProducer::onRegisterFailed, this, _1, _2));
        m_face.setInterestFilter("/F",
                                 std::bind(&BaseProducer::onInterest, this, _2),
                                 nullptr, // RegisterPrefixSuccessCallback is optional
                                 std::bind(&BaseProducer::onRegisterFailed, this, _1, _2));
        m_face.setInterestFilter("/G",
                                 std::bind(&BaseProducer::onInterest, this, _2),
                                 nullptr, // RegisterPrefixSuccessCallback is optional
                                 std::bind(&BaseProducer::onRegisterFailed, this, _1, _2));
        m_face.setInterestFilter("/H",
                                 std::bind(&BaseProducer::onInterest, this, _2),
                                 nullptr, // RegisterPrefixSuccessCallback is optional
                                 std::bind(&BaseProducer::onRegisterFailed, this, _1, _2));

        auto cert = m_keyChain.getPib().getDefaultIdentity().getDefaultKey().getDefaultCertificate();
        m_certServeHandle = m_face.setInterestFilter(
            security::extractIdentityFromCertName(cert.getName()),
            [this, cert](auto &&...)
            {
              m_face.put(cert);
            },
            std::bind(&MyProducer::onRegisterFailed, this, _1, _2));
        m_face.processEvents();
      }
    };

  } // namespace examples
} // namespace ndn

int main(int argc, char **argv)
{
  try
  {
    ndn::examples::MyProducer producer;
    producer.run();
    return 0;
  }
  catch (const std::exception &e)
  {
    std::cerr << "ERROR: " << e.what() << std::endl;
    return 1;
  }
}
